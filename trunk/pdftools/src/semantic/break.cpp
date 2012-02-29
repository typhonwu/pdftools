#include "break.h"

void Break::do_glyph(Html *document, Context *context)
{
    document->break_line();
}
