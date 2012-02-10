#include "fontnode.h"

FontNode::FontNode() : TreeNode()
{
}

string FontNode::name()
{
    return m_name;
}

int FontNode::size()
{
    return m_size;
}

void FontNode::set_name(string name)
{
    m_name = name;
}

void FontNode::set_size(int size)
{
    m_size = size;
}
