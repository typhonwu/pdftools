#pragma once
#ifndef TEXTMATRIXGLYPH_H
#define TEXTMATRIXGLYPH_H

#include "glyph.h"

class TextMatrixGlyph : public Glyph {
private:
double m_a;
    double m_b;
    double m_c;
    double m_d;
    double m_e;
    double m_f;
    
public:
    TextMatrixGlyph(double a, double b, double c, double d, double e, double f);
    virtual void do_glyph(Html *document);
};

#endif
