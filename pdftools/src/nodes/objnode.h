#ifndef OBJNODE_H
#define OBJNODE_H

#include "treenode.h"
#include <vector>
#include <stdint.h>

class ObjNode : public TreeNode {
private:
    int m_id;
    int m_generation;
    TreeNode *m_value;
    uint8_t *m_stream;
    
public:
    ObjNode(int id, int generation);
    virtual ~ObjNode();
    
    void set_value(TreeNode *value);
    void set_stream(std::vector<uint8_t> stream);
    
    TreeNode *value();
    uint8_t *stream();
    bool this_object(int id, int generation);
};

#endif
