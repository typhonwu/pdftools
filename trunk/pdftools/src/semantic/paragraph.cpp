#include "paragraph.h"

void Paragraph::start_glyph(Html *document, Context *context)
{
    document->add_paragraph();
}

void Paragraph::end_glyph(Html *document, Context *context)
{
    document->end_tag();
}
