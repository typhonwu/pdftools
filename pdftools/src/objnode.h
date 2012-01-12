#ifndef OBJNODE_H
#define OBJNODE_H

#include "treenode.h"

class ObjNode : public TreeNode {
private:
    int id;
    int generator;
    
public:
    ObjNode(int id, int generator);
    virtual ~ObjNode();
};

#endif
