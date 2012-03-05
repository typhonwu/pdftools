#include "statenode.h"

StateNode::StateNode(bool save) : TreeNode()
{
    m_save = save;
}

bool StateNode::save()
{
    return m_save;
}
