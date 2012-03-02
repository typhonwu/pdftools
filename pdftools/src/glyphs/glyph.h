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

protected:
    Context *m_context;

public:
    Glyph();
    virtual ~Glyph();
    void add_child(Glyph *glyph);
    void execute(Html *document, Context *context);

    virtual void do_glyph(Html *document);
    virtual void start_glyph(Html *document);
    virtual void end_glyph(Html *document);
};

#endif
