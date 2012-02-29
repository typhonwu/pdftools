#ifndef BREAK_H
#define BREAK_H

#include "glyph.h"

class Break : public Glyph {
public:
    virtual void do_glyph(Html *document, Context *context);
};

#endif
