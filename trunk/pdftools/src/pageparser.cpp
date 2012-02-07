#include "pageparser.h"
#include "utils.h"
#include "nodes/treenode.h"

PageParser::PageParser(istream &stream)
{
    m_scanner.set_istream(&stream);
    m_scanner.set_ignore_newchar(false);
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
    bool error = false;
    next_token();

    while (m_scanner.good() && !error) {
        switch (m_token->type()) {
        case BT:
            bt_sequence(root);
            break;
        default:
            next_token();
            error = true;
        }
    }
    return root;
}

#include <iostream>

void PageParser::bt_sequence(RootNode *root)
{
    match(BT);

    vector<Token *> tokens;
    while (m_token->type() != ET) {
        Token *token = new Token;
        token->set_type(m_token->type());
        token->set_value(m_token->value());
        if (token->type() == NEW_LINE) {
            // process tokens
            cout << tokens[tokens.size() - 1]->value() << endl;
            
            tokens.clear();
        } else {
            tokens.push_back(token);
        }
        next_token();
    }
    match(ET);
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
