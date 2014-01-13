#include "fontsizeglyph.h"
#include "../semantic/context.h"

FontSizeGlyph::FontSizeGlyph(double size) : Glyph()
{
    m_size = size;
}

void FontSizeGlyph::do_glyph(Html *document)
{
    m_context->set_font_size(m_size);
}
