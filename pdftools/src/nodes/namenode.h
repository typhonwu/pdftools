#ifndef NAMENODE_H
#define NAMENODE_H

#include <string>
#include "treenode.h"

class NameNode : public TreeNode {
private:
    std::wstring m_name;

public:
    NameNode(std::wstring name);
    virtual ~NameNode();
};

#endif
