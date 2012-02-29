#pragma once
#ifndef GLYPH_H
#define GLYPH_H

#include "html/html.h"
#include <vector>

using namespace std;

class Context;

class Glyph {
private:
    vector<Glyph *> m_childs;

public:
    Glyph();
    virtual ~Glyph();
    void add_child(Glyph *glyph);
    void execute(Html *document, Context *context);

    virtual void do_glyph(Html *document, Context *context);
    virtual void start_glyph(Html *document, Context *context);
    virtual void end_glyph(Html *document, Context *context);
};

#endif
