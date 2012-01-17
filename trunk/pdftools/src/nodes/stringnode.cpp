#include "stringnode.h"

StringNode::StringNode(std::wstring value) : TreeNode()
{
    m_value = value;
}

StringNode::~StringNode()
{

}
