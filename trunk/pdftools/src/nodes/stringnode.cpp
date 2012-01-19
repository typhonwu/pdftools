#include "stringnode.h"

StringNode::StringNode(string value) : TreeNode()
{
    m_value = value;
}

StringNode::~StringNode()
{
}

string StringNode::value()
{
    return m_value;
}
