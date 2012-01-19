#ifndef MAPNODE_H
#define MAPNODE_H

#include "treenode.h"
#include <map>
#include <string>

class MapNode : public TreeNode {
private:
    std::map<std::string, TreeNode *> m_values;
    
public:
    MapNode();
    ~MapNode();
    
    void push(std::string name, TreeNode *value);
    std::map<std::string, TreeNode *> values();
};

#endif
