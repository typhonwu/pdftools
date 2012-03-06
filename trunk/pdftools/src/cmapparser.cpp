#include "cmapparser.h"
#include "utils.h"
#include "scanner.h"
#include "token.h"
#include "nodes/nodes.h"
#include <cstring>

CMapParser::CMapParser(istream *stream)
{
    m_scanner->disable_charset_conversion();
    m_scanner->set_istream(stream);
    m_root = NULL;
}

CMapParser::~CMapParser()
{
    if (m_root) {
        delete m_root;
    }
}

CMapNode *CMapParser::parse()
{
    int val;

    if (m_root) {
        delete m_root;
    }
    m_root = new CMapNode();

    next_token();

    // /CIDInit
    match(NAME);
    // /ProcSet
    match(NAME);
    // findresource
    match(NAME);
    // begin
    match(NAME);
    // 12
    match(NUM);
    // dict
    match(NAME);
    // begin
    match(NAME);
    //begincmap
    match(NAME);

    while (m_scanner->good()) {
        switch (m_token->type()) {
        case NAME:
            if (m_token->value() == "/CMapName") {
                match(NAME);
                // /Name
                match(NAME);
                // def
                match(NAME);
            } else if (m_token->value() == "/CMapType") {
                match(NAME);
                // 2
                match(NUM);
                // def
                match(NAME);
            } else if (m_token->value() == "/CIDSystemInfo") {
                match(NAME);
                delete value_sequence();
                // def
                match(NAME);
            } else {
                match(NAME);
            }
            break;
        case NUM:
            val = m_token->to_number();
            match(NUM);
            if (m_token->value() == "beginbfchar") {
                match(NAME);
                bfchar_sequence(val);
            } else if (m_token->value() == "begincodespacerange") {
                match(NAME);
                m_root->set_codespace(codespace_sequence());
            } else if (m_token->value() == "beginbfrange") {
                match(NAME);
                bfrange_sequence(val);
            } else {
                match(NAME);
                error_message("invalid mode");
            }
            break;
        default:
            next_token();
            break;
        }
    }
    return m_root;
}

CodeSpaceNode *CMapParser::codespace_sequence()
{
    CodeSpaceNode *ret = new CodeSpaceNode;
    ret->set_start(m_token->value());
    match(STRING);
    ret->set_finish(m_token->value());
    match(STRING);
    // endcodespacerange
    match(NAME);
    return ret;
}

void CMapParser::bfchar_sequence(const int count)
{
    for (int loop = 0; loop < count; loop++) {
        string character = m_token->value();
        match(STRING);
        string unicode = m_token->value();
        match(STRING);
        m_root->add(new CharNode(character, unicode));
    }
    match(NAME);
}

void CMapParser::bfrange_sequence(const int count)
{
    for (int loop = 0; loop < count; loop++) {
        string start = m_token->value();
        match(STRING);
        string end = m_token->value();
        match(STRING);
        TreeNode *node = value_sequence();
        StringNode *name = dynamic_cast<StringNode *>(node);
        if (name) {
            char *chars = const_cast<char *>(start.c_str());
            char *b = chars;
            b++;
            int size = start.size();

            // TODO find a better solution
            while (strcmp(chars, end.c_str()) <= 0) {
                m_root->add(new CharNode(string(chars, size), name->value()));
                if (size == 1) {
                    (*chars)++;
                } else {
                    uint16_t c = (*chars << 8) + (*b & 255);
                    c++;
                    *chars = c >> 8;
                    *b = c & 0xFF;
                }
            }
        } else {
            error_message("test map");
            ArrayNode *array = dynamic_cast<ArrayNode *>(node);
            char *chars = const_cast<char *>(start.c_str());
            int size = start.size();
            int loop2 = 0;

            while (memcmp(chars, end.c_str(), size) < 0) {
                name = dynamic_cast<StringNode *>(array->value(loop2));
                m_root->add(new CharNode(string(chars, size), name->value()));
                if (size == 1) {
                    (*chars)++;
                } else {
                    uint16_t *c = (uint16_t *) chars;
                    (*c)++;
                }
                loop2++;
            }
        }
    }
    match(NAME);
}
