#include "codespacenode.h"

CodeSpaceNode::CodeSpaceNode() : TreeNode()
{
}

void CodeSpaceNode::set_start(string start)
{
    m_start = start;
}

void CodeSpaceNode::set_finish(string finish)
{
    m_finish = finish;
}

string CodeSpaceNode::start()
{
    return m_start;
}

string CodeSpaceNode::finish()
{
    return m_finish;
}
