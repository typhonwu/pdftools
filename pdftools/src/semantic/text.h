#ifndef TEXT_H
#define TEXT_H

#include "glyph.h"
#include <string>

using namespace std;

class Text : public Glyph {
private:
    string m_text;
    
public:
    Text(string text);
    virtual void do_glyph(Html *document, Context *context);
};

#endif
