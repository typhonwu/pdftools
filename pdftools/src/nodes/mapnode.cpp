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
    m_values.clear();
}

map<std::string, TreeNode *> MapNode::values()
{
    return m_values;
}

void MapNode::push(std::string name, TreeNode *value)
{
    m_values[name] = value;
}
