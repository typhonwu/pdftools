#include "arraynode.h"

using namespace std;

ArrayNode::ArrayNode() : TreeNode()
{
}

ArrayNode::~ArrayNode()
{
    vector<TreeNode*>::iterator i = m_values.begin();
    while (i != m_values.end()) {
        delete *i;
        i++;
    }
}

TreeNode *ArrayNode::value(int index)
{
    return m_values[index];
}

int ArrayNode::size()
{
    return m_values.size();
}

void ArrayNode::push(TreeNode *value)
{
    m_values.push_back(value);
}
