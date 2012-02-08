#ifndef COMMANDNODE_H
#define COMMANDNODE_H

#include "treenode.h"
#include <vector>

using namespace std;

class CommandNode : public TreeNode {
private:
    vector<TreeNode *> m_parameters;
    TreeNode *m_command;

public:
    CommandNode();
    ~CommandNode();
    
    int size();
    TreeNode *parameter(int index);
    TreeNode *command();
    void add_parameter(TreeNode *parameter);
};

#endif
