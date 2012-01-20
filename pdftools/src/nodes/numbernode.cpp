#include "numbernode.h"

NumberNode::NumberNode(double value) : TreeNode()
{
    m_value = value;
}

NumberNode::~NumberNode()
{
}

double NumberNode::value()
{
    return m_value;
}
