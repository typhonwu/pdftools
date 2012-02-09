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
    void execute(Html *document);
};

#endif
