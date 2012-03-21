#pragma once
#ifndef FONTSIZEGLYPH_H
#define FONTSIZEGLYPH_H

#include "glyph.h"

class FontSizeGlyph : public Glyph {
private:
    double m_size;

public:
    FontSizeGlyph(double size);
    
    virtual void do_glyph(Html *document);
};

#endif
