#pragma once
#ifndef TEXTGLYPH_H
#define TEXTGLYPH_H

#include "glyph.h"
#include <string>

using namespace std;

class TextGlyph : public Glyph {
private:
    string m_text;
    
public:
    TextGlyph(string text);
    virtual void do_glyph(Html *document);
};

#endif
