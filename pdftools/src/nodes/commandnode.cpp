#include "commandnode.h"
#include <cstdlib>

CommandNode::CommandNode() : TreeNode()
{
    m_command = NULL;
}

CommandNode::~CommandNode()
{
    if (m_command) {
        delete m_command;
    }
    vector<TreeNode *>::iterator i;
    for (i = m_parameters.begin(); i != m_parameters.end(); i++) {
        delete *i;
    }
}

void CommandNode::add_parameter(TreeNode *parameter)
{
    if (m_command) {
        m_parameters.push_back(m_command);
    }
    m_command = parameter;
}

int CommandNode::size()
{
    return m_parameters.size();
}

TreeNode *CommandNode::parameter(int index)
{
    return m_parameters[index];
}

TreeNode *CommandNode::command()
{
    return m_command;
}
