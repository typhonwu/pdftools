#pragma once
#ifndef BDCCOMMAND_H
#define BDCCOMMAND_H

#include "rootnode.h"
#include <string>

using namespace std;

class BDCNode : public RootNode {
private:
    TreeNode *m_value;
    string m_name;
    
public:
    BDCNode();
    ~BDCNode();
    
    TreeNode *value();
    string name();
    void set_value(TreeNode *value);
    void set_name(string name);
};

#endif