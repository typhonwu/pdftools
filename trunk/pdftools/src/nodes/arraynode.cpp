#include "arraynode.h"

using namespace std;

ArrayNode::ArrayNode() : TreeNode()
{
}

ArrayNode::~ArrayNode()
{
    vector<TreeNode*>::iterator i;
    for (i = m_values.begin(); i != m_values.end(); i++) {
        delete *i;
    }
    m_values.clear();
}

void ArrayNode::push(TreeNode *value)
{
    m_values.push_back(value);
}