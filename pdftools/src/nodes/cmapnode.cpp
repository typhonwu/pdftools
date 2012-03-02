#include "cmapnode.h"
#include "codespacenode.h"
#include "charnode.h"

CMapNode::CMapNode() : TreeNode()
{
    m_codespace = NULL;
}

CMapNode::~CMapNode()
{
    vector<CharNode *>::iterator i;
    for (i = m_charnodes.begin(); i != m_charnodes.end(); i++) {
        delete *i;
    }
    if (m_codespace) {
        delete m_codespace;
    }
}

void CMapNode::add(CharNode *node)
{
    m_charnodes.push_back(node);
}

void CMapNode::set_codespace(CodeSpaceNode *codespace)
{
    m_codespace = codespace;
}

CodeSpaceNode *CMapNode::code_space()
{
    return m_codespace;
}

int CMapNode::nodes()
{
    return m_charnodes.size();
}

CharNode *CMapNode::node(int index)
{
    return m_charnodes.at(index);
}
