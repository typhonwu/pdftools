#ifndef STRINGNODE_H
#define STRINGNODE_H

#include <string>
#include "treenode.h"

class StringNode : public TreeNode {
private:
    std::string m_value;

public:
    StringNode(std::string value);
    virtual ~StringNode();
};

#endif
