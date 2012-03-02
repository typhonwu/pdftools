#include "fontglyph.h"
#include "semantic/context.h"

FontGlyph::FontGlyph(string font, int size) : Glyph()
{
    m_font = font;
    m_size = size;
}

int FontGlyph::size()
{
    return m_size;
}

string FontGlyph::font()
{
    return m_font;
}

void FontGlyph::do_glyph(Html *document)
{
    m_context->set_current_font(m_font, m_size);
}
