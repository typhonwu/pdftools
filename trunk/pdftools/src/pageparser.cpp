#include "pageparser.h"
#include "utils.h"
#include "token.h"
#include "nodes/nodes.h"
#include "glyphs/glyphs.h"
#include <iostream>

using namespace std;

PageParser::PageParser(istream *stream) :
        GenericParser()
{
    m_scanner->disable_charset_conversion();
    m_scanner->set_istream(stream);
    m_root = NULL;
}

PageParser::~PageParser()
{
    if (m_root) {
        delete m_root;
    }
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
    while (m_scanner->good()) {
        TreeNode *value = value_sequence();
        if (value) {
            values.push_back(value);
        } else {
            switch (m_token->type()) {
            case BDC:
                m_root->add_child(bdc_sequence(values));
                break;
            case BI:
                m_root->add_child(bi_sequence());
                break;

            case TF:
                m_root->add_child(font_sequence(values));
                break;

                // Graphic State
            case Q_LO:
                m_root->add_child(new StateNode(true));
                next_token();
                break;
            case Q_UP:
                m_root->add_child(new StateNode(false));
                next_token();
                break;

                // Text
            case TJ_LO:
                match(TJ_LO);
                m_root->add_child(text_sequence(values));
                break;
            case TJ_UP:
                tjup_sequence(m_root, values);
                break;
            case T_AST:
                match(T_AST);
                m_root->add_child(new BreakNode);
                break;
            case DOUBLE_QUOTE: {
                match(DOUBLE_QUOTE);
                m_root->add_child(new BreakNode);
                vector<TreeNode *> vector;
                vector.assign(values.begin() + 3, values.end());
                m_root->add_child(text_sequence(vector));
                break;
            }
            case QUOTE:
                match(QUOTE);
                m_root->add_child(new BreakNode);
                m_root->add_child(text_sequence(values));
                next_token();
                break;
            default:
                next_token();
                break;
            }
            register int size = values.size();
            for (register int loop = 0; loop < size; loop++) {
                if (values[loop]) {
                    delete values[loop];
                }
            }
            values.clear();
        }

    }
    return m_root;
}

TreeNode *PageParser::font_sequence(vector<TreeNode *> &values)
{
    match(TF);

    if (values.size() == 2) {
        FontNode *font = new FontNode;

        NameNode *font_name = dynamic_cast<NameNode *>(values[0]);
        if (font_name) {
            font->set_name(font_name->name());
        }
        NumberNode *font_size = dynamic_cast<NumberNode *>(values[1]);
        if (font_size) {
            font->set_size(font_size->value());
        }
        return font;
    } else {
        error_message("Error parsing font object");
        return NULL;
    }
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
    m_scanner->get_image_stream();
    next_token();
    match(EI);
    return NULL;
}

TreeNode *PageParser::text_sequence(vector<TreeNode *> &values)
{
    TextNode *text = new TextNode;
    int size = values.size();
    for (int loop = 0; loop < size; loop++) {
        StringNode *node = dynamic_cast<StringNode *>(values[loop]);
        if (node) {
            text->add(node->value());
        }
    }
    return text;
}

void PageParser::tjup_sequence(RootNode *root, vector<TreeNode *> &values)
{
    match(TJ_UP);
    int size = values.size();
    for (int loop = 0; loop < size; loop++) {
        ArrayNode *array = dynamic_cast<ArrayNode *>(values[loop]);
        if (array) {
            int array_size = array->size();
            for (int y = 0; y < array_size; y++) {
                StringNode *node = dynamic_cast<StringNode *>(array->value(y));
                if (node) {
                    TextNode *text = new TextNode;
                    text->add(node->value());
                    root->add_child(text);
                }
            }
        }
    }
}

TreeNode *PageParser::bdc_sequence(vector<TreeNode *> &values)
{
    match(BDC);
    BDCNode *node = new BDCNode;

    NameNode *name = dynamic_cast<NameNode *>(values[0]);
    if (name) {
        node->set_name(name->name());
    }
    node->set_value(values[1]);
    // avoid double delete
    values[1] = NULL;

    return node;
}
