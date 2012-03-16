#include "textmatrixnode.h"

TextMatrixNode::TextMatrixNode(double a, double b, double c, double d, double e, double f) : TreeNode()
{
    m_a = a;
    m_b = b;
    m_c = c;
    m_d = d;
    m_e = e;
    m_f = f;
}

double TextMatrixNode::a()
{
    return m_a;
}

double TextMatrixNode::b()
{
    return m_b;
}

double TextMatrixNode::c()
{
    return m_c;
}

double TextMatrixNode::d()
{
    return m_d;
}

double TextMatrixNode::e()
{
    return m_e;
}

double TextMatrixNode::f()
{
    return m_f;
}
