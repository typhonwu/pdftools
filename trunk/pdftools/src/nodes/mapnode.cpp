#include <stdexcept>
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
    try {
        return m_values.at(name);
    } catch (out_of_range &) {
        return NULL;
    }
}

map<string, TreeNode *> MapNode::values()
{
    return m_values;
}

void MapNode::push(string name, TreeNode *value)
{
    m_values[name] = value;
}
