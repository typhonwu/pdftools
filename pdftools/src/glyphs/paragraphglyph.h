#pragma once
#ifndef PARAGRAPHGLYPH_H
#define PARAGRAPHGLYPH_H

#include "glyph.h"

class ParagraphGlyph : public Glyph {
public:
    virtual void start_glyph(Html *document);
    virtual void end_glyph(Html *document);
};

#endif
