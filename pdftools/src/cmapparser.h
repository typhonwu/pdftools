#pragma once
#ifndef CMAPPARSER_H
#define CMAPPARSER_H

#include "scanner.h"
#include <istream>

using namespace std;

class TreeNode;
class CMapNode;
class CodeSpaceNode;

class CMapParser {
private:
    Scanner m_scanner;
    Token *m_token;
    CMapNode *m_root;

public:
    CMapParser(istream *stream);
    ~CMapParser();

    CMapNode *parse();

private:
    void next_token();
    bool match(TokenType type);
    TreeNode *value_sequence();
    void bfchar_sequence(const int count);
    void bfrange_sequence(const int count);
    CodeSpaceNode *codespace_sequence();
};

#endif
