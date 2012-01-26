#include "mapnode.h"
#include "treenode.h"

using namespace std;

MapNode::MapNode() : TreeNode()
{
}

MapNode::~MapNode()
{
    map<string, TreeNode*>::iterator i = m_values.begin();
    while (i != m_values.end()) {
        delete (*i).second;
        i++;
    }
}

TreeNode *MapNode::get(string name)
{
    return m_values[name];
}


map<string, TreeNode *> MapNode::values()
{
    return m_values;
}

void MapNode::push(string name, TreeNode *value)
{
    m_values[name] = value;
}
