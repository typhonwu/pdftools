#include "namenode.h"

NameNode::NameNode(std::string name) : TreeNode()
{
    m_name = name;
}

NameNode::~NameNode()
{

}

string NameNode::name()
{
    return m_name;
}
