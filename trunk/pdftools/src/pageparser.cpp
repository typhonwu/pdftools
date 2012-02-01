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

RootNode *PageParser::parse()
{
    RootNode *root = new RootNode();
    bool error = false;
    next_token();

    while (m_scanner.good() && !error) {
        switch (m_token->type()) {
        case NAME:
            if (m_token->value() == "BT") {
                bt_sequence();
            } else {
                error_message("Invalid text start");
            }
            break;
        default:
            next_token();
            error = true;
        }
    }
    return root;
}

void PageParser::bt_sequence()
{
    match(NAME);
    
    while(!(m_token->type() == NAME && m_token->value() == "ET")) {
        
    }
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
