#ifndef ARRAYNODE_H
#define ARRAYNODE_H

#include "treenode.h"
#include <vector>

using namespace std;

class ArrayNode : public TreeNode {
private:
    vector<TreeNode *> m_values;

public:
    ArrayNode();
    virtual ~ArrayNode();

    void push(TreeNode *value);
    TreeNode *value(int index);
    int size();
};

#endif
