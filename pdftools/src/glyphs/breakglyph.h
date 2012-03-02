#pragma once
#ifndef BREAKGLYPH_H
#define BREAKGLYPH_H

#include "glyph.h"

class BreakGlyph : public Glyph {
public:
    virtual void do_glyph(Html *document);
};

#endif
