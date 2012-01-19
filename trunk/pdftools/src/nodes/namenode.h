#ifndef NAMENODE_H
#define NAMENODE_H

#include <string>
#include "treenode.h"

using namespace std;

class NameNode : public TreeNode {
private:
    string m_name;

public:
    NameNode(string name);
    virtual ~NameNode();
    
    string name();
};

#endif
