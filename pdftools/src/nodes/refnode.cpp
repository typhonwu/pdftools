#include "refnode.h"

RefNode::RefNode(int id, int generation)
{
    m_id = id;
    m_generation = generation;
}

RefNode::~RefNode()
{

}
