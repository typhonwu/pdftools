#ifndef MAPNODE_H
#define MAPNODE_H

#include "treenode.h"
#include <map>

class MapNode : public TreeNode {
private:
    std::map<std::wstring, TreeNode *> m_values;
    
public:
    MapNode();
    ~MapNode();
    
    void push(std::wstring name, TreeNode *value);
};

#endif
