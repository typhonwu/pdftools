#include "objnode.h"
#include <cstdlib>

using namespace std;

ObjNode::ObjNode(int id, int generator) : TreeNode()
{
    this->id = id;
    this->generator = generator;
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

void ObjNode::set_value(TreeNode *value)
{
    m_value = value;
}

void ObjNode::set_stream(vector<uint8_t> stream)
{
    m_stream = new uint8_t[stream.size()];
    copy(stream.begin(), stream.end(), m_stream);
}
