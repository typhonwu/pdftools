#ifndef OBJNODE_H
#define OBJNODE_H

#include "treenode.h"
#include <vector>
#include <stdint.h>

using namespace std;

class ObjNode : public TreeNode {
private:
    int m_id;
    int m_generation;
    int m_stream_size;
    TreeNode *m_value;
    int8_t *m_stream;
    char *m_uncompressed;
    
public:
    ObjNode(int id, int generation);
    virtual ~ObjNode();
    
    void set_value(TreeNode *value);
    void set_stream(pair<int, int8_t *> stream);
    
    int id();
    int generation();
    int stream_size();
    TreeNode *value();
    int8_t *stream();
    char *uncompressed();
    void set_uncompressed(char *data);
    bool this_object(int id, int generation);
    void clear_stream();
};

#endif
