#include "paragraphglyph.h"
#include "semantic/font.h"
#include "html/html.h"
#include "semantic/context.h"

void ParagraphGlyph::start_glyph(Html *document)
{
    document->add_paragraph();
}

void ParagraphGlyph::end_glyph(Html *document)
{
    document->end_tag();
}
