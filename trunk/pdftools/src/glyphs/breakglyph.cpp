#include "breakglyph.h"
#include "../html/html.h"

void BreakGlyph::do_glyph(Html *document)
{
    document->break_line();
}
