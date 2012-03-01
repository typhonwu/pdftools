#include "cmapparser.h"
#include "utils.h"
#include "nodes/nodes.h"

CMapParser::CMapParser(istream *stream)
{
    m_scanner.disable_chatset_conversion();
    m_scanner.set_istream(stream);
    m_root = NULL;
}

CMapParser::~CMapParser()
{
    if (m_root) {
        delete m_root;
    }
}

void CMapParser::next_token()
{
    m_token = m_scanner.next_token();
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

    while (m_scanner.good()) {
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
}

bool CMapParser::match(TokenType type)
{
    if (m_token && m_token->type() == type) {
        next_token();
    } else {
#ifdef DEBUG
        string msg = "unexpected token: ";
        if (m_token) {
            msg += m_token->value();
        }
        error_message(msg.c_str());
#endif
        next_token();
        return false;
    }
    return true;
}

TreeNode *CMapParser::value_sequence()
{
    if (m_token->type() == START_DICT) {
        match(START_DICT);
        MapNode *map = new MapNode;

        while (m_scanner.good() && m_token && m_token->type() != END_DICT) {
            string name = m_token->value();
            match(NAME);
            TreeNode *value = value_sequence();
            NameNode *n = dynamic_cast<NameNode *> (value);
            if (n && n->name()[0] != '/') {
                value = value_sequence();
            }
            map->push(name, value);
        }
        match(END_DICT);
        return map;
    } else if (m_token->type() == TRUE) {
        match(TRUE);
        return new BooleanNode(true);
    } else if (m_token->type() == FALSE) {
        match(FALSE);
        return new BooleanNode(false);
    } else if (m_token->type() == NAME) {
        string name = m_token->value();
        match(NAME);
        return new NameNode(name);
    } else if (m_token->type() == STRING) {
        string value = m_token->value();
        match(STRING);
        return new StringNode(value);
    } else if (m_token->type() == NUM) {
        double value = m_token->to_number();
        int pos = m_scanner.pos();
        match(NUM);

        if (m_token->type() == NUM) {
            double generation = m_token->to_number();
            match(NUM);
            if (m_token->type() == NAME && m_token->value() == "R") {
                match(NAME);
                return new RefNode(value, generation);
            } else {
                m_scanner.to_pos(pos);
            }
        } else {
            m_scanner.to_pos(pos);
        }
        next_token();
        return new NumberNode(value);
    } else if (m_token->type() == START_ARRAY) {
        ArrayNode *array = new ArrayNode;
        match(START_ARRAY);
        while (m_scanner.good() && m_token->type() != END_ARRAY) {
            array->push(value_sequence());
        }
        match(END_ARRAY);
        return array;
    }
    return NULL;
}
