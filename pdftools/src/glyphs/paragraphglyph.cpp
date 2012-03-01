#include "paragraphglyph.h"

void ParagraphGlyph::start_glyph(Html *document)
{
    document->add_paragraph();
}

void ParagraphGlyph::end_glyph(Html *document)
{
    document->end_tag();
}
