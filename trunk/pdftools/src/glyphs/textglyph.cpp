#include "textglyph.h"
#include "html/html.h"
#include "semantic/context.h"
#include "semantic/font.h"
#include <cstring>

TextGlyph::TextGlyph(string text)
{
    m_text = text;
}

void TextGlyph::do_glyph(Html *document)
{
    Font *current = m_context->font();

    string text;
    if (current) {
        text = current->translate(m_text);
    } else {
        text = m_text;
    }
    document->add_element(text.c_str());
}

void TextGlyph::start_glyph(Html *document)
{
    if (m_context->font_changed()) {
        if (m_context->use_font()) {
            document->end_tag();
        }
        Font *current = m_context->font();
        if (current) {
            document->add_font(current->size() * m_context->state()->get_text_font(), current->bold(), current->italic(), current->fixed());
        }
        m_context->set_font_changed(false);
        m_context->set_use_font(true);
    }
}
