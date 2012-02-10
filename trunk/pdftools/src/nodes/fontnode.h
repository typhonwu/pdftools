#pragma once
#ifndef FONTNODE_H
#define FONTNODE_H

#include "treenode.h"
#include <string>

using namespace std;

class FontNode : public TreeNode {
private:
    string m_name;
    int m_size;

public:
    FontNode();
    
    string name();
    int size();
    void set_name(string name);
    void set_size(int size);
};

#endif
