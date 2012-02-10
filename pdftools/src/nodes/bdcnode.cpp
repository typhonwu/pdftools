#include "bdcnode.h"

BDCNode::BDCNode() : RootNode()
{
    m_value = NULL;
}

BDCNode::~BDCNode()
{
    if (m_value) {
        delete m_value;
    }
}

TreeNode *BDCNode::value()
{
    return m_value;
}

string BDCNode::name()
{
    return m_name;
}

void BDCNode::set_value(TreeNode *value)
{
    m_value = value;
}

void BDCNode::set_name(string name)
{
    m_name = name;
}
