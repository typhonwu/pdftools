#ifndef REFNODE_H
#define REFNODE_H

#include "treenode.h"

class RefNode : public TreeNode {
private:
    int m_id;
    int m_generation;

public:
    RefNode(int id, int generation);
    ~RefNode();
};

#endif
