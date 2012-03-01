#include "textglyph.h"

TextGlyph::TextGlyph(string text)
{
    m_text = text;
}

void TextGlyph::do_glyph(Html *document)
{
    document->add_element(m_text.c_str());
}
