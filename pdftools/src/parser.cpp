#include "parser.h"
#include "scanner.h"
#include "utils.h"
#include <nodes.h>
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

const wchar_t *_pdf_versions[] = {
    L"PDF-1.1",
    L"PDF-1.2",
    L"PDF-1.3",
    L"PDF-1.4",
    L"PDF-1.5",
    L"PDF-1.6",
    L"PDF-1.7"
};

Parser::Parser() : m_token(NULL), m_valid(false), m_linear(false)
{
    m_scanner = new Scanner();
}

Parser::~Parser()
{
    delete m_scanner;
    if (m_token) {
        delete m_token;
    }
}

bool Parser::open_file(const char *path)
{
    bool ret = m_scanner->open_file(path);
    if (ret) {
        next_token();
    }
    return ret;
}

void Parser::next_token()
{
    if (m_token) {
        delete m_token;
    }
    m_token = m_scanner->next_token();
}

bool Parser::match(TokenType type)
{
    if (m_token && m_token->type() == type) {
        next_token();
    } else {
        wstring msg = L"unexpected token: ";
        msg += m_token->value();
        error_message(msg.c_str());
        next_token();
        return false;
    }
    return true;
}

RootNode *Parser::parse()
{
    RootNode *root = new RootNode();
    bool error = false;
    match(PERCENT);
    if (verify_version()) {
        while (m_scanner->good() && !error) {
            if (m_token == NULL) {
                break;
            }
            switch (m_token->type()) {
            case PERCENT:
                comment_sequence();
                break;
            case NUM:
                root->add_child(object_sequence());
                break;
            case XREF:
                root->add_child(xref_sequence());
                break;
            default:
                next_token();
                error = true;
            }
        }
    } else {
        error_message(L"invalid input file");
    }
    return root;
}

void Parser::comment_sequence()
{
    m_scanner->ignore_line();
    next_token();
}

TreeNode * Parser::xref_sequence()
{
    XREFNode *xref = new XREFNode;
    match(XREF);

    do {
        uint16_t id = (uint16_t) m_token->to_number();
        match(NUM);
        int count = (int) m_token->to_number();
        match(NUM);

        for (int loop = 0; loop < count; loop++) {
            uint32_t address = (int) m_token->to_number();
            match(NUM);
            uint16_t generation = (int) m_token->to_number();
            match(NUM);
            wstring name = m_token->value();
            match(NAME);
            xref->add_node(id, generation, address, name.at(0));
            id++;
        }
    } while (m_scanner->good() && (m_token->type() != TRAILER));
    match(TRAILER);
    xref->set_trailer(value_sequence());

    match(START_XREF);
    xref->set_start_address(m_token->to_number());
    match(NUM);

    match(PERCENT);
    match(PERCENT);
    match(END_PDF);
    return xref;
}

TreeNode *Parser::value_sequence()
{
    if (m_token->type() == START_DICT) {
        match(START_DICT);
        MapNode *map = new MapNode;

        while (m_scanner->good() && m_token->type() != END_DICT) {
            wstring name = m_token->value();
            match(NAME);
            TreeNode *value = value_sequence();
            map->push(name, value);
        }
        match(END_DICT);
        return map;
    } else if (m_token->type() == NAME) {
        wstring name = m_token->value();
        match(NAME);
        return new NameNode(name);
    } else if (m_token->type() == STRING) {
        wstring value = m_token->value();
        match(STRING);
        return new StringNode(value);
    } else if (m_token->type() == NUM) {
        double value = m_token->to_number();
        int pos = m_scanner->pos();
        match(NUM);

        if (m_token->type() == NUM) {
            double generation = m_token->to_number();
            match(NUM);
            if (m_token->type() == NAME && m_token->value() == L"R") {
                match(NAME);
                return new RefNode(value, generation);
            } else {
                m_scanner->to_pos(pos);
            }
        } else {
            m_scanner->to_pos(pos);
        }
        next_token();
        return new NumberNode(value);
    } else if (m_token->type() == START_ARRAY) {
        ArrayNode *array = new ArrayNode;
        match(START_ARRAY);
        while (m_scanner->good() && m_token->type() != END_ARRAY) {
            array->push(value_sequence());
        }
        match(END_ARRAY);
        return array;
    }
    return NULL;
}

TreeNode *Parser::object_sequence()
{
    float number = m_token->to_number();
    match(NUM);
    float generation_nunber = m_token->to_number();
    match(NUM);

    ObjNode *node = new ObjNode((int) number, (int) generation_nunber);
    match(OBJ);
    node->set_value(value_sequence());
    if (m_token->type() == STREAM) {
        node->set_stream(m_scanner->get_stream());
        next_token();
        match(END_STREAM);
    }
    match(END_OBJ);

    return node;
}

bool Parser::is_valid()
{
    return m_valid;
}

bool Parser::verify_version()
{
    int loop;
    wstring line = m_token->value();

    for (loop = 0; loop < 7; loop++) {
        if (line == _pdf_versions[loop]) {
            m_version = _pdf_versions[loop];
            match(NAME);
            return true;
        }
    }
    return false;
}

