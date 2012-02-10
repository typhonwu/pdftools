#pragma once
#ifndef TEXTNODE_H
#define TEXTNODE_H

#include "treenode.h"
#include <string>

using namespace std;

class TextNode : public TreeNode {
private:
    string m_value;
    
public:
    TextNode();
    void add(string text);
    string text();
};

#endif
