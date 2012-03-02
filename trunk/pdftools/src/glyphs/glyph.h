#pragma once
#ifndef GLYPH_H
#define GLYPH_H

#include <vector>

using namespace std;

class Html;
class Context;

class Glyph {
private:
    vector<Glyph *> m_childs;

protected:
    Context *m_context;
    Glyph *m_last_glyph;

public:
    Glyph();
    virtual ~Glyph();
    void add_child(Glyph *glyph);
    void execute(Html *document, Context *context);

    virtual void do_glyph(Html *document);
    virtual void start_glyph(Html *document);
    virtual void end_glyph(Html *document);

    void set_last(Glyph *glyph);
};

#endif
