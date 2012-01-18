#include "xrefnode.h"

using namespace std;

XREFNode::XREFNode() : TreeNode()
{
    m_start_address = 0;
    m_trailer = 0;
}

XREFNode::~XREFNode()
{
    if (m_trailer) {
        delete m_trailer;
    }
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

long XREFNode::start_address()
{
    return m_start_address;
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
