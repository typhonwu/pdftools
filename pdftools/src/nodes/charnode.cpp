#include "charnode.h"

CharNode::CharNode(string character, string unicode) : TreeNode()
{
    m_char = character;
    m_unicode = unicode;
}

string CharNode::character()
{
    return m_char;
}

string CharNode::unicode()
{
    return m_unicode;
}
