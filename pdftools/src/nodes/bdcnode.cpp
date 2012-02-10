#include "bdcnode.h"

BDCNode::BDCNode(TreeNode *value) : RootNode()
{
    m_value = value;
}

TreeNode *BDCNode::value()
{
    return m_value;
}
