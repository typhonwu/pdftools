#pragma once
#ifndef BDCCOMMAND_H
#define BDCCOMMAND_H

#include "rootnode.h"

class BDCNode : public RootNode {
private:
    TreeNode *m_value;
    
public:
    BDCNode(TreeNode *value);
    
    TreeNode *value();
};

#endif