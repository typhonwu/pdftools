#pragma once
#ifndef PARAGRAPH_H

#include "glyph.h"

class Paragraph : public Glyph {
public:
    virtual void start_glyph(Html *document);
    virtual void end_glyph(Html *document);
};

#endif
