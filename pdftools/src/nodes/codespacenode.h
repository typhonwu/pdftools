#pragma once
#ifndef CODESPACENODE_H
#define CODESPACENODE_H

#include "treenode.h"
#include <string>

using namespace std;

class CodeSpaceNode : TreeNode {
private:
    string m_start;
    string m_finish;
    
public:
    CodeSpaceNode();
    
    void set_start(string start);
    void set_finish(string finish);
    
    string start();
    string finish();
};

#endif
