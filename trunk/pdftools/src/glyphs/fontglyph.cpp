#include "fontglyph.h"
#include "../semantic/font.h"
#include "../html/html.h"
#include "../semantic/context.h"

FontGlyph::FontGlyph(string font, int size) : Glyph()
{
    m_font = font;
    m_size = size;
}

void FontGlyph::do_glyph(Html *)
{
    m_context->set_current_font(m_font, m_size);
}
