#include "paragraph.h"

void Paragraph::start_glyph(Html *document)
{
    document->add_paragraph();
}

void Paragraph::end_glyph(Html *document)
{
    document->end_tag();
}
