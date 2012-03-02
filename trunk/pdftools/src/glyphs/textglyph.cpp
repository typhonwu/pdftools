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

    const char *text;
    if (current) {
        text = current->translate(m_text).c_str();
    } else {
        text = m_text.c_str();
    }
    document->add_element(text);
}
