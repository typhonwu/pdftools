#pragma once
#ifndef GENERICPARSER_H
#define GENERICPARSER_H

#include "scanner.h"

class TreeNode;

class GenericParser {
protected:
    Scanner *m_scanner;
    Token *m_token;

public:
    GenericParser();
    virtual ~GenericParser();

protected:
    TreeNode *value_sequence();
    bool match(TokenType type);
    void next_token();
};

#endif
