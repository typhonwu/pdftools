#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include "token.h"

class TreeNode {
private:
    std::vector<TreeNode *> m_child;
    TreeNode *m_sibling;

public:
    TreeNode();
    virtual ~TreeNode();
    
    void add_child(TreeNode *child);
};

#endif
