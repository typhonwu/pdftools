#ifndef OBJNODE_H
#define OBJNODE_H

#include "treenode.h"
#include <stdint.h>

class ObjNode : public TreeNode {
private:
    int id;
    int generator;
    TreeNode *m_value;
    uint8_t *m_stream;
    
public:
    ObjNode(int id, int generator);
    virtual ~ObjNode();
    
    void set_value(TreeNode *value);
    void set_stream(uint8_t *stream);
};

#endif
