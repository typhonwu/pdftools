#pragma once
#ifndef TEXTMATRIXNODE_H
#define TEXTMATRIXNODE_H

#include "treenode.h"

class TextMatrixNode : public TreeNode {
private:
    double m_a;
    double m_b;
    double m_c;
    double m_d;
    double m_e;
    double m_f;
    
public:
    TextMatrixNode(double a, double b, double c, double d, double e, double f);
    
    double a();
    double b();
    double c();
    double d();
    double e();
    double f();
};

#endif
