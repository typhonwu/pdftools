#include "xrefnode.h"

XREFNode::XREFNode() : TreeNode()
{
    m_start_address = NULL;
}

XREFNode::~XREFNode()
{
    m_references.clear();
}

void XREFNode::set_start_address(long address)
{
    m_start_address = address;
}

void XREFNode::set_trailer(TreeNode *trailer)
{
    m_trailer = trailer;
}

void XREFNode::add_node(uint16_t id, uint16_t generation, uint32_t address, wchar_t status)
{
    object_reference xref;
    xref.id = id;
    xref.generation = generation;
    xref.address = address;
    xref.status = status;
    m_references.push_back(xref);
}
