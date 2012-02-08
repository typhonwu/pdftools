#include "rootnode.h"

using namespace std;

RootNode::RootNode() : TreeNode()
{
}

RootNode::~RootNode()
{
    vector<TreeNode*>::iterator i = m_child.begin();
    while (i != m_child.end()) {
        delete *i;
        i++;
    }
}

void RootNode::add_child(TreeNode *child)
{
    m_child.push_back(child);
}

TreeNode *RootNode::get(int index)
{
    return m_child[index];
}

int RootNode::size()
{
    return m_child.size();
}
