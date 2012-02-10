#include "textnode.h"

TextNode::TextNode() : TreeNode()
{
}

void TextNode::add(string text)
{
    m_value += text;
}

string TextNode::text()
{
    return m_value;
}
