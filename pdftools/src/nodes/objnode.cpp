#include "objnode.h"
#include <cstdlib>

using namespace std;

ObjNode::ObjNode(int id, int generation) : TreeNode()
{
    m_id = id;
    m_generation = generation;
    m_value = NULL;
}

ObjNode::~ObjNode()
{
    if (m_value) {
        delete m_value;
    }
}

int ObjNode::id()
{
    return m_id;
}

int ObjNode::generation()
{
    return m_generation;
}

TreeNode *ObjNode::value()
{
    return m_value;
}

bool ObjNode::this_object(int id, int generation)
{
    return m_id == id && m_generation == generation;
}

void ObjNode::set_value(TreeNode *value)
{
    m_value = value;
}

void ObjNode::set_stream_pos(int pos)
{
    m_stream_pos = pos;
}

int ObjNode::stream_pos()
{
    return m_stream_pos;
}
