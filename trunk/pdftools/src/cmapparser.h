#pragma once
#ifndef CMAPPARSER_H
#define CMAPPARSER_H

#include "scanner.h"
#include "token.h"
#include "nodes/nodes.h"
#include <istream>

using namespace std;

class CMapParser {
private:
    Scanner m_scanner;
    Token *m_token;
    RootNode *m_root;

public:
    CMapParser(istream *stream);
    ~CMapParser();

    RootNode *parse();

private:
    void next_token();
    bool match(TokenType type);
    TreeNode *value_sequence();

};

#endif
