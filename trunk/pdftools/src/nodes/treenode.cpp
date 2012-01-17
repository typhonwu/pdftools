#include "treenode.h"

using namespace std;

TreeNode::TreeNode()
{

}

TreeNode::~TreeNode()
{
    vector<TreeNode*>::iterator i;
    for (i = m_child.begin(); i != m_child.end(); i++) {
        delete *i;
    }
}

void TreeNode::add_child(TreeNode *child)
{
    m_child.push_back(child);
}
