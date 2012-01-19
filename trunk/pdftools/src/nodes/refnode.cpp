#include "refnode.h"

RefNode::RefNode(int id, int generation) : TreeNode()
{
    m_id = id;
    m_generation = generation;
}

RefNode::~RefNode()
{

}

int RefNode::id()
{
    return m_id;
}

int RefNode::generation()
{
    return m_generation;
}
