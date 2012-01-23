#include "objnode.h"
#include <cstdlib>

using namespace std;

ObjNode::ObjNode(int id, int generation) : TreeNode()
{
    m_id = id;
    m_generation = generation;
    m_value = NULL;
    m_stream = NULL;
    m_uncompressed = NULL;
}

ObjNode::~ObjNode()
{
    if (m_value) {
        delete m_value;
    }
    if (m_stream) {
        delete [] m_stream;
    }
    if (m_uncompressed) {
        delete [] m_uncompressed;
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

int8_t *ObjNode::stream()
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

int ObjNode::stream_size()
{
    return m_stream_size;
}

void ObjNode::clear_stream()
{
    if (m_stream) {
        delete [] m_stream;
        m_stream = NULL;
    }
}

void ObjNode::set_stream(vector<int8_t> stream)
{
    m_stream_size = stream.size();
    m_stream = new int8_t[stream.size()];
    copy(stream.begin(), stream.end(), m_stream);
}

char *ObjNode::uncompressed()
{
    return m_uncompressed;
}

void ObjNode::set_uncompressed(char *data)
{
    m_uncompressed = data;
}
