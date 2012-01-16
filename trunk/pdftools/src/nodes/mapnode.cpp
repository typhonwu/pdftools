#include "mapnode.h"
#include "treenode.h"

using namespace std;

MapNode::MapNode() : TreeNode()
{
    
}

MapNode::~MapNode()
{
    map<wstring, TreeNode*>::iterator i;
    for (i = m_values.begin(); i != m_values.end(); i++) {
        delete (*i).second;
    }
    m_values.clear();
}

void MapNode::push(std::wstring name, TreeNode *value)
{
    m_values[name] = value;
}
