#include "rootnode.h"

using namespace std;

RootNode::RootNode() : TreeNode()
{

}

RootNode::~RootNode()
{
    vector<TreeNode*>::iterator i;
    for (i = m_child.begin(); i != m_child.end(); i++) {
        delete *i;
    }
}

void RootNode::add_child(TreeNode *child)
{
    m_child.push_back(child);
}
