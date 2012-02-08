#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include "scanner.h"
#include "token.h"
#include "nodes/nodes.h"
#include <istream>

using namespace std;

class PageParser {
private:
    Scanner m_scanner;
    Token *m_token;

public:
    PageParser(istream &stream);
    ~PageParser();
    
    RootNode *parse();
    
private:
    void next_token();
    bool match(TokenType type);
    
    TreeNode *value_sequence();
};

#endif
