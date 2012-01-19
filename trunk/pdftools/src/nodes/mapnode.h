#ifndef MAPNODE_H
#define MAPNODE_H

#include "treenode.h"
#include <map>
#include <string>

using namespace std;

class MapNode : public TreeNode {
private:
    map<string, TreeNode *> m_values;
    
public:
    MapNode();
    ~MapNode();
    
    void push(string name, TreeNode *value);
    map<string, TreeNode *> values();
    
    TreeNode *get(string name);
};

#endif
