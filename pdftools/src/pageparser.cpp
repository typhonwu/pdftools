#include "pageparser.h"
#include "utils.h"
#include "nodes/treenode.h"
#include <iostream>

PageParser::PageParser(istream *stream)
{
    m_scanner.set_istream(stream);
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
        case BI:
            return bi_sequence();
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
        case T_AST:
            match(T_AST);
            // new line
            break;
        case W_UP:
            match(W_UP);
            // Command
            next_token();
            break;
        case W_AST:
            match(W_AST);
            // Command
            next_token();
            break;
        case Q_LO:
            match(Q_LO);
            return NULL;
            break;
        case Q_UP:
            match(Q_UP);
            return NULL;
            break;
        case F_AST:
            match(F_AST);
            return NULL;
            break;
        case F_LO:
            match(F_LO);
            return NULL;
            break;
        case F_UP:
            match(F_UP);
            return NULL;
            break;
        case S_LO:
            match(S_LO);
            return NULL;
            break;
        case S_UP:
            match(S_UP);
            return NULL;
            break;
        case H:
            match(H);
            return NULL;
            break;
        default:
            next_token();
        }
    }
    return NULL;
}

TreeNode *PageParser::bi_sequence()
{
    match(BI);
    while(m_token->type() != ID) {
        string name = m_token->value();
        match(NAME);
        TreeNode *value = value_sequence();
        delete value;
    }
    match(ID);
    while(m_token->type() != EI && m_scanner.good()) {
        next_token();
    }
    match(EI);
    return NULL;
}

TreeNode *PageParser::text_sequence()
{
    string num1 = m_token->value();
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
    case G_LO:
        match(G_LO);
        return NULL;
        break;
    case G_UP:
        match(G_UP);
        return NULL;
        break;
    case I:
        match(I);
        return NULL;
        break;
    case J_LO:
        match(J_LO);
        return NULL;
        break;
    case J_UP:
        match(J_UP);
        return NULL;
        break;
    case W_LO:
        match(W_LO);
        return NULL;
        break;
    case M_UP:
        match(M_UP);
        return NULL;
        break;
    case F_AST:
        match(F_AST);
        return NULL;
        break;
    default:
        break;
    }
    string num2 = m_token->value();
    match(NUM);

    switch (m_token->type()) {
    case M_LO:
        match(M_LO);
        return NULL;
        break;
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
    case L:
        match(L);
        return NULL;
        break;
    default:
        break;
    }
    string num3 = m_token->value();
    match(NUM);

    if (m_token->type() == SCN) {
        match(SCN);
        return NULL;
    } else if (m_token->type() == RG_LO) {
        match(RG_LO);
        return NULL;
    } else if (m_token->type() == RG_UP) {
        match(RG_UP);
        return NULL;
    }
    string num4 = m_token->value();
    match(NUM);

    if (m_token->type() == SCN) {
        match(SCN);
        return NULL;
    } else if (m_token->type() == RE) {
        match(RE);
        return NULL;
    } else if (m_token->type() == K_UP) {
        match(K_UP);
        return NULL;
    } else if (m_token->type() == K_LO) {
        match(K_LO);
        return NULL;
    } else if (m_token->type() == V) {
        match(V);
        return NULL;
    } else if (m_token->type() == Y) {
        match(Y);
        return NULL;
    }
    string num5 = m_token->value();
    match(NUM);

    if (m_token->type() == SCN) {
        match(SCN);
        return NULL;
    }
    string num6 = m_token->value();
    match(NUM);
    
    if (m_token->type() == TM) {
        match(TM);
    } else if (m_token->type() == C) {
        match(C);
    } else {
        match(CM);
    }
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
    if (m_token->type() == NUM) {
        // FIXME d sequence
        match(NUM);
        match(D);
    } else {
        match(TJ_UP);
    }
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
    } else if (m_token->type() == CS) {
        match(CS);
        // Ignore color space
        return NULL;
    } else if (m_token->type() == SCN) {
        match(SCN);
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
