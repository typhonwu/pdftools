#include "xrefnode.h"

using namespace std;

XREFNode::XREFNode() : TreeNode()
{
    m_trailer = 0;
}

XREFNode::~XREFNode()
{
    if (m_trailer) {
        delete m_trailer;
    }
}

void XREFNode::set_trailer(TreeNode *trailer)
{
    m_trailer = trailer;
}

TreeNode *XREFNode::trailer()
{
    return m_trailer;
}

void XREFNode::add_node(uint16_t id, uint16_t generation, uint32_t address, char status)
{
    object_reference xref;
    xref.id = id;
    xref.generation = generation;
    xref.address = address;
    xref.status = status;
    m_references.push_back(xref);
}
