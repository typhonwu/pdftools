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
        *i = NULL;
        i = m_values.erase(i);
    }
}

void ArrayNode::push(TreeNode *value)
{
    m_values.push_back(value);
}