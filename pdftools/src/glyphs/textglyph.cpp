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
