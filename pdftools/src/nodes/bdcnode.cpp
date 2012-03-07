#include "bdcnode.h"

BDCNode::BDCNode(RootNode *parent) : RootNode()
{
    m_value = NULL;
    m_parent = parent;
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

RootNode *BDCNode::parent()
{
    return m_parent;
}
