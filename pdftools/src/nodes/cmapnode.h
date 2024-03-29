#pragma once
#ifndef CMAPNODE_H
#define CMAPNODE_H

#include "treenode.h"
#include <vector>

using namespace std;

class CodeSpaceNode;
class CharNode;

class CMapNode : public TreeNode {
private:
    vector<CharNode *> m_charnodes;
    CodeSpaceNode *m_codespace;

public:
    CMapNode();
    ~CMapNode();
    
    void add(CharNode *node);
    void set_codespace(CodeSpaceNode *codespace);
    
    CodeSpaceNode *code_space();
    int nodes();
    CharNode *node(int index);
};

#endif
