#include "textglyph.h"
#include "html/html.h"
#include "semantic/context.h"
#include "semantic/font.h"

TextGlyph::TextGlyph(string text)
{
    m_text = text;
}

void TextGlyph::do_glyph(Html *document)
{
    Font *current = m_context->font();

    if (current) {
        document->add_element(current->translate(m_text).c_str());
    } else {
        document->add_element(m_text.c_str());
    }
}
