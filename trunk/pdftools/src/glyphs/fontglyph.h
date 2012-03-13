#pragma once
#ifndef FONTGLYPH_H
#define FONTGLYPH_H

#include <string>
#include "glyph.h"

using namespace std;

class FontGlyph: public Glyph {
private:
    int m_size;
    string m_font;
    Glyph *m_parent;

public:
    FontGlyph(string font, int size);

    virtual void start_glyph(Html *document);
    virtual void end_glyph(Html *document);

    void set_parent(Glyph *parent);
    Glyph *parent();
    int size();
    string font();
};

#endif
