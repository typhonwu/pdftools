#include "fontglyph.h"
#include "semantic/font.h"
#include "html/html.h"
#include "semantic/context.h"

FontGlyph::FontGlyph(string font, int size) :
        Glyph()
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

void FontGlyph::set_parent(Glyph *parent)
{
    m_parent = parent;
}

Glyph *FontGlyph::parent()
{
    return m_parent;
}

#include <iostream>

void FontGlyph::start_glyph(Html *document)
{
    m_context->set_current_font(m_font, m_size);
    Font *font = m_context->font();
    //cout << font->name() << endl;
    document->add_font(m_size, font->bold(), font->italic(), font->fixed());
}

void FontGlyph::end_glyph(Html *document)
{
    document->end_tag();
}
