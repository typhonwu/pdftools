#pragma once
#ifndef CMAPPARSER_H
#define CMAPPARSER_H

#include "scanner.h"
#include "genericparser.h"
#include <istream>

using namespace std;

class TreeNode;
class CMapNode;
class CodeSpaceNode;

class CMapParser : public GenericParser {
private:
    CMapNode *m_root;

public:
    CMapParser(istream *stream);
    ~CMapParser();

    CMapNode *parse();

private:
    void bfchar_sequence(const int count);
    void bfrange_sequence(const int count);
    CodeSpaceNode *codespace_sequence();
};

#endif
