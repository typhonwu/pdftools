#include "objnode.h"
#include <cstdlib>

using namespace std;

ObjNode::ObjNode(int id, int generation) : TreeNode()
{
    m_id = id;
    m_generation = generation;
    m_value = NULL;
    m_stream = NULL;
}

ObjNode::~ObjNode()
{
    if (m_value) {
        delete m_value;
    }
    if (m_stream) {
        delete [] m_stream;
    }
}

TreeNode *ObjNode::value()
{
    return m_value;
}

uint8_t *ObjNode::stream()
{
    return m_stream;
}

bool ObjNode::this_object(int id, int generation)
{
    return m_id == id && m_generation == generation;            
}

void ObjNode::set_value(TreeNode *value)
{
    m_value = value;
}

void ObjNode::set_stream(vector<uint8_t> stream)
{
    m_stream = new uint8_t[stream.size()];
    copy(stream.begin(), stream.end(), m_stream);
}
