#include "paragraphglyph.h"
#include "semantic/font.h"
#include "html/html.h"
#include "semantic/context.h"

void ParagraphGlyph::start_glyph(Html *document)
{
    document->add_paragraph();
//    Font *current = m_context->font();
//    if (current) {
//        m_context->set_font_changed(true);
//    }
}

void ParagraphGlyph::end_glyph(Html *document)
{
//    if (m_context->use_font()) {
//        document->end_tag();
//        m_context->set_use_font(false);
//    }
    document->end_tag();
}
