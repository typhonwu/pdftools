#ifndef NUMBERNODE_H
#define NUMBERNODE_H

#include "treenode.h"

class NumberNode : public TreeNode {
private:
    double m_value;

public:
    NumberNode(double value);
    ~NumberNode();
};

#endif