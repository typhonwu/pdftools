#ifndef OBJNODE_H
#define OBJNODE_H

#include "treenode.h"

class ObjNode : public TreeNode {
private:
    int id;
    int generator;
    TreeNode *m_value;
    
public:
    ObjNode(int id, int generator);
    virtual ~ObjNode();
    
    void set_value(TreeNode *value);
};

#endif
