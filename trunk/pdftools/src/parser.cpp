#include "parser.h"
#include "scanner.h"
#include "utils.h"
#include "nodes/nodes.h"
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

const char *_pdf_versions[] = {
    "PDF-1.1",
    "PDF-1.2",
    "PDF-1.3",
    "PDF-1.4",
    "PDF-1.5",
    "PDF-1.6",
    "PDF-1.7"
};

Parser::Parser(const char *filein)
{
    m_token = NULL;
    m_linear = false;
    m_scanner = new Scanner;
    m_filein = filein;
    
    m_filestream.open(filein, ios::binary);
    m_scanner->set_istream(&m_filestream);
    
    if (is_valid()) {
        next_token();
    }
}

Parser::~Parser()
{
    if (m_scanner) {
        delete m_scanner;
    }
    m_filestream.close();
}

void Parser::next_token()
{
    m_token = m_scanner->next_token();
}

bool Parser::match(TokenType type)
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

RootNode *Parser::parse()
{
    RootNode *root = new RootNode();
    bool error = false;
    match(PERCENT);
    if (verify_version()) {
        while (m_scanner->good() && !error) {
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
            case START_XREF:
                startxref_sequence();
                break;
            default:
                next_token();
                error = true;
                break;
            }
        }
    } else {
        error_message("invalid input file");
    }
    m_filestream.close();
    m_filestream.open(m_filein, ios::binary);
    m_scanner->set_istream(&m_filestream);
    object_streams(root);
    m_filestream.close();
    return root;
}

void Parser::object_streams(RootNode *root_node)
{
    int size = root_node->size();
    int i;

    for (i = 0; i < size; i++) {
        ObjNode *root_object = dynamic_cast<ObjNode *> (root_node->get(i));
        if (root_object) {
            MapNode *map = dynamic_cast<MapNode *> (root_object->value());
            if (map) {
                NameNode *type = dynamic_cast<NameNode *> (map->get("/Type"));
                if (type && type->name() == "/ObjStm") {
                    int qtd = 0;
                    int length = 0;
                    NumberNode *number = dynamic_cast<NumberNode *> (map->get("/N"));
                    if (number) {
                        qtd = number->value();
                    }
                    NumberNode *length_node = dynamic_cast<NumberNode *> (map->get("/Length"));
                    if (number) {
                        length = length_node->value();
                    }
                    char *uncompressed = NULL;
                    
                    m_scanner->to_pos(root_object->stream_pos());
                    char *stream = (char *)m_scanner->get_stream(length);

                    int total = length;
                    NameNode *filter = dynamic_cast<NameNode *> (map->get("/Filter"));
                    if (filter && filter->name() == "/FlateDecode") {
                        uncompressed = flat_decode(stream, length, total);
                        delete [] stream;
                    } else if (!filter) {
                        uncompressed = stream;
                    } else {
                        error_message(string("compression not supported: ") + filter->name());
                        return;
                    }
                    stringstream stream_value;
                    stream_value.write(uncompressed, total);
                    stream_value.seekg(0);
                    delete [] uncompressed;

                    Scanner scanner;
                    Scanner *temp = m_scanner;
                    m_scanner = &scanner;
                    scanner.set_istream(&stream_value);

                    vector<int> ids;
                    int loop;
                    for (loop = 0; loop < qtd; loop++) {
                        next_token();
                        ids.push_back(m_token->to_number());
                        next_token();
                    }
                    next_token();
                    vector<int>::iterator id;
                    for (id = ids.begin(); id < ids.end(); id++) {
                        ObjNode *new_obj = new ObjNode(*id, 0);
                        new_obj->set_value(value_sequence());
                        root_node->add_child(new_obj);
                    }
                    m_scanner = temp;
                }
            }
        }
    }
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
            string name = m_token->value();
            if (m_token->type() == F_LO) {
                match(F_LO);
            } else {
                match(N);
            }
            xref->add_node(id, generation, address, name.at(0));
            id++;
        }
    } while (m_scanner->good() && (m_token->type() != TRAILER));
    match(TRAILER);
    xref->set_trailer(value_sequence());
    return xref;
}

void Parser::startxref_sequence()
{
    match(START_XREF);
    match(NUM);

    match(PERCENT);
    match(PERCENT);
    match(END_PDF);
}

TreeNode *Parser::value_sequence()
{
    if (m_token->type() == START_DICT) {
        match(START_DICT);
        MapNode *map = new MapNode;

        while (m_scanner->good() && m_token && m_token->type() != END_DICT) {
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
        int pos = m_scanner->pos();
        match(NUM);

        if (m_token->type() == NUM) {
            double generation = m_token->to_number();
            match(NUM);
            if (m_token->type() == NAME && m_token->value() == "R") {
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
    if (m_token && m_token->type() == STREAM) {
        int length = 0;
        MapNode *map = dynamic_cast<MapNode *> (node->value());
        if (map) {
            NumberNode *number = dynamic_cast<NumberNode *> (map->get("/Length"));
            if (number) {
                length = number->value();
            }
        }
        node->set_stream_pos(m_scanner->ignore_stream(length));
        next_token();
        match(END_STREAM);
    }
    match(END_OBJ);

    return node;
}

bool Parser::is_valid()
{
    return m_filestream.is_open();
}

bool Parser::verify_version()
{
    int loop;
    if (m_token) {
        string line = m_token->value();
        for (loop = 0; loop < 7; loop++) {
            if (line == _pdf_versions[loop]) {
                m_version = _pdf_versions[loop];
                match(NAME);
                return true;
            }
        }
    }
    return false;
}

