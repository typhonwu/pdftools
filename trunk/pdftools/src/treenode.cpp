#include "treenode.h"

using namespace std;

TreeNode::TreeNode()
{

}

TreeNode::~TreeNode()
{
    vector<TreeNode*>::iterator i;
    for (i = child.begin(); i != child.end(); i++) {
        delete *i;
    }
}
