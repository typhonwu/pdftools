#ifndef STRINGNODE_H
#define STRINGNODE_H

#include <string>
#include "treenode.h"

class StringNode : public TreeNode {
private:
    std::wstring m_value;

public:
    StringNode(std::wstring value);
    virtual ~StringNode();
};

#endif
