#include "objnode.h"

ObjNode::ObjNode(int id, int generator) : TreeNode()
{
    this->id = id;
    this->generator = generator;
    m_value = NULL;
}

ObjNode::~ObjNode()
{
    if (m_value) {
        delete m_value;
    }
}

void ObjNode::set_value(TreeNode *value)
{
    m_value = value;
}
