#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include "token.h"

class TreeNode {
private:
    std::vector<TreeNode *> child;
    TreeNode *sibling;

public:
    TreeNode();
    virtual ~TreeNode();
};

#endif
