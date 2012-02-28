#pragma once
#ifndef CHARNODE_H
#define CHARNODE_H

#include <string>
#include "treenode.h"

using namespace std;

class CharNode : public TreeNode {
private:
    string m_char;
    string m_unicode;
    
public:
    CharNode(string character, string unicode);
    
    string character();
    string unicode();
};

#endif
