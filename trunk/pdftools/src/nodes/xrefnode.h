#ifndef XREFNODE_H
#define XREFNODE_H

#include "treenode.h"
#include <vector>
#include <stdint.h>

typedef struct {
    uint32_t address;
    uint16_t id;
    uint16_t generation;
    char status;
} object_reference;

class XREFNode : public TreeNode {
private:
    std::vector<object_reference> m_references;
    TreeNode *m_trailer;

public:
    XREFNode();
    virtual ~XREFNode();

    void add_node(uint16_t id, uint16_t generation, uint32_t address, char status);
    void set_trailer(TreeNode *trailer);

    TreeNode *trailer();
};

#endif
