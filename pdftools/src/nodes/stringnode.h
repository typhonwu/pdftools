#pragma once
#ifndef STRINGNODE_H
#define STRINGNODE_H

#include <string>
#include "treenode.h"

using namespace std;

class StringNode : public TreeNode {
private:
    string m_value;

public:
    StringNode(string value);
    virtual ~StringNode();
    
    string value();
};

#endif
