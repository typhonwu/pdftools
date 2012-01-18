#ifndef NAMENODE_H
#define NAMENODE_H

#include <string>
#include "treenode.h"

class NameNode : public TreeNode {
private:
    std::string m_name;

public:
    NameNode(std::string name);
    virtual ~NameNode();
};

#endif
