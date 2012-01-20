#include "booleannode.h"

BooleanNode::BooleanNode(bool value)
{
    m_value = value;
}

BooleanNode::~BooleanNode()
{
}

bool BooleanNode::value()
{
    return m_value;
}
