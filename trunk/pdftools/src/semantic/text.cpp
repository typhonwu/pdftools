#include "text.h"

Text::Text(string text)
{
    m_text = text;
}

void Text::do_glyph(Html *document)
{
    document->add_element(m_text.c_str());
}
