#ifndef ROOTNODE_H
#define ROOTNODE_H

#include <vector>
#include "treenode.h"

class RootNode : TreeNode {
private:
    std::vector<TreeNode *> m_child;

public:
    RootNode();
    virtual ~RootNode();
    
    void add_child(TreeNode *child);
};

#endif
