#include "objnode.h"

ObjNode::ObjNode(int id, int generator) : TreeNode()
{
    this->id = id;
    this->generator = generator;
}

ObjNode::~ObjNode()
{
    
}
