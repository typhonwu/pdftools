#include "pageparser.h"
#include "utils.h"
#include "nodes/treenode.h"

PageParser::PageParser(istream &stream)
{
    m_scanner.set_istream(&stream);
}

PageParser::~PageParser()
{
}

void PageParser::next_token()
{
    m_token = m_scanner.next_token();
}

// 196

RootNode *PageParser::parse()
{
    RootNode *root = new RootNode();

    next_token();
    while (m_scanner.good()) {
        root->add_child(sequence());
    }
    return root;
}

TreeNode *PageParser::sequence()
{
    while (m_scanner.good()) {
        switch (m_token->type()) {
        case BT:
            return bt_sequence();
            break;
        case NAME:
            return bdc_sequence();
            break;
        case NUM:
            return text_sequence();
            break;
        case STRING:
            return tjlo_sequence();
            break;
        case START_ARRAY:
            return tjup_sequence();
            break;
        default:
            next_token();
        }
    }
    return NULL;
}

TreeNode *PageParser::text_sequence()
{
    //double num1 = m_token->to_number();
    match(NUM);

    switch (m_token->type()) {
    case TC:
        match(TC);
        return NULL;
        break;
    case TW:
        match(TW);
        return NULL;
        break;
    case TZ:
        match(TZ);
        return NULL;
        break;
    case TL:
        match(TL);
        return NULL;
        break;
    case TR:
        match(TR);
        return NULL;
        break;
    case TS:
        match(TS);
        return NULL;
        break;
    case SCN:
        match(SCN);
        return NULL;
        break;
    default:
        break;
    }
    //double num2 = m_token->to_number();
    match(NUM);

    switch (m_token->type()) {
    case TD_LO:
        match(TD_LO);
        return NULL;
        break;
    case TD_HI:
        match(TD_HI);
        return NULL;
        break;
    case SCN:
        match(SCN);
        return NULL;
        break;
    default:
        break;
    }
    //double num3 = m_token->to_number();
    match(NUM);
    
    if (m_token->type() == SCN) {
        match(SCN);
        return NULL;
    }
    //double num4 = m_token->to_number();
    match(NUM);
    
    if (m_token->type() == SCN) {
        match(SCN);
        return NULL;
    }
    //double num5 = m_token->to_number();
    match(NUM);
    
    if (m_token->type() == SCN) {
        match(SCN);
        return NULL;
    }
    //double num6 = m_token->to_number();
    match(NUM);
    match(TM);
    return NULL;
}

TreeNode *PageParser::bt_sequence()
{
    match(BT);

    BTNode *bt = new BTNode;
    while (m_token->type() != ET && m_scanner.good()) {
        bt->add_child(sequence());
    }
    return bt;
}

TreeNode *PageParser::tjlo_sequence()
{
    string value = m_token->value();
    match(STRING);
    match(TJ_LO);

    TextNode *text = new TextNode;
    text->add(value);
    return text;
}

TreeNode *PageParser::tjup_sequence()
{
    match(START_ARRAY);

    TextNode *text = new TextNode;
    while (m_token->type() != END_ARRAY && m_scanner.good()) {
        if (m_token->type() == STRING) {
            text->add(m_token->value());
            match(STRING);
        } else {
            match(NUM);
        }
    }
    match(END_ARRAY);
    match(TJ_UP);

    return text;
}

TreeNode *PageParser::bdc_sequence()
{
    string name = m_token->value();
    match(NAME);

    if (m_token->type() == GS) {
        match(GS);
        // Ignore graphic state
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

    BDCNode *bdc = new BDCNode;
    bdc->set_value(value);
    bdc->set_name(name);
    while (m_token->type() != EMC && m_scanner.good()) {
        bdc->add_child(sequence());
    }
    return bdc;
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
