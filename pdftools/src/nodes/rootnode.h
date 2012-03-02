#pragma once
#ifndef ROOTNODE_H
#define ROOTNODE_H

#include <vector>
#include "treenode.h"

using namespace std;

class RootNode : public TreeNode {
private:
    vector<TreeNode *> m_child;

public:
    RootNode();
    virtual ~RootNode();
    
    void add_child(TreeNode *child);
    TreeNode *get(int index);
    int size();
};

#endif
