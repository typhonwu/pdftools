#pragma once
#ifndef STATENODE_H
#define STATENODE_H

#include "treenode.h"

class StateNode : public TreeNode {
private:
    bool m_save;

public:
    StateNode(bool save);

    bool save();
};

#endif
