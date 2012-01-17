#ifndef ARRAYNODE_H
#define ARRAYNODE_H

#include "treenode.h"
#include <vector>

class ArrayNode : public TreeNode {
private:
    std::vector<TreeNode *> m_values;

public:
    ArrayNode();
    virtual ~ArrayNode();
    
    void push(TreeNode *value);
};

#endif
