#include "pageparser.h"
#include "utils.h"
#include "nodes/treenode.h"
#include "semantic/glyph.h"
#include "semantic/break.h"
#include <iostream>
#include <vector>

using namespace std;

PageParser::PageParser(istream *stream)
{
    m_scanner.set_istream(stream);
    m_root = NULL;
}

PageParser::~PageParser()
{
    if (m_root) {
        delete m_root;
    }
}

void PageParser::next_token()
{
    m_token = m_scanner.next_token();
}

RootNode *PageParser::parse()
{
    if (m_root) {
        delete m_root;
    }
    m_root = new RootNode();
    vector<TreeNode *> values;
    values.reserve(10);

    next_token();
    while (m_scanner.good()) {
        //        root->add_child(sequence());
        TreeNode *value = value_sequence();
        if (value) {
            values.push_back(value);
        } else {
            switch (m_token->type()) {
            case BI:
                m_root->add_child(bi_sequence());
                break;
            case TJ_LO:
                m_root->add_child(tjlo_sequence(values));
                break;
            case TJ_UP:
                m_root->add_child(tjup_sequence(values));
                break;
            case T_AST:
                m_root->add_child(new TextNode);
                next_token();
                break;
            case QUOTE:
                m_root->add_child(new TextNode);
                m_root->add_child(tjlo_sequence(values));
                next_token();
                break;
            default:
                next_token();
            }
            register int size = values.size();
            for (register int loop = 0; loop < size; loop++) {
                delete values[loop];
            }
            values.clear();
        }

    }
    return m_root;
}

TreeNode *PageParser::bi_sequence()
{
    match(BI);
    while (m_token->type() != ID) {
        string name = m_token->value();
        match(NAME);
        TreeNode *value = value_sequence();
        delete value;
    }
    m_scanner.get_image_stream();
    next_token();
    match(EI);
    return NULL;
}

TreeNode *PageParser::tjlo_sequence(vector<TreeNode *> &values)
{
    match(TJ_LO);
    TextNode *text = new TextNode;
    int size = values.size();
    for (int loop = 0; loop < size; loop++) {
        StringNode *node = dynamic_cast<StringNode *> (values[loop]);
        if (node) {
            text->add(node->value());
        }
    }
    return text;
}

TreeNode *PageParser::tjup_sequence(vector<TreeNode *> &values)
{
    match(TJ_UP);
    RootNode *g = new RootNode;
    int size = values.size();
    for (int loop = 0; loop < size; loop++) {
        ArrayNode *array = dynamic_cast<ArrayNode *> (values[loop]);
        if (array) {
            int array_size = array->size();
            for (int y = 0; y < array_size; y++) {
                StringNode *node = dynamic_cast<StringNode *> (array->value(y));
                if (node) {
                    TextNode *text = new TextNode;
                    text->add(node->value());
                    g->add_child(text);
                }
            }
        }
    }
    return g;
}

TreeNode *PageParser::bdc_sequence()
{
    string name = m_token->value();
    match(NAME);

    if (m_token->type() == GS) {
        match(GS);
        // Ignore graphic state
        return NULL;
    } else if (m_token->type() == CS_UP) {
        match(CS_UP);
        // Ignore color space
        return NULL;
    } else if (m_token->type() == SCN_UP) {
        match(SCN_UP);
        // Ignore color space
        return NULL;
    } else if (m_token->type() == DO) {
        match(DO);
        // Ignore XObject
        return NULL;
    }

    TreeNode *value = value_sequence();
    NumberNode *size = dynamic_cast<NumberNode *> (value);
    if (size) {
        match(TF);
        FontNode *font = new FontNode;
        font->set_name(name);
        font->set_size(size->value());
        return font;
    }
    return NULL;
}

bool PageParser::match(TokenType type)
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

TreeNode *PageParser::value_sequence()
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
