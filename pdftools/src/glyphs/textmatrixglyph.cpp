#include "textmatrixglyph.h"
#include "semantic/page.h"
#include "semantic/context.h"
#include "graphics/graphicstate.h"

TextMatrixGlyph::TextMatrixGlyph(double a, double b, double c, double d, double e, double f) : Glyph()
{
    m_a = a;
    m_b = b;
    m_c = c;
    m_d = d;
    m_e = e;
    m_f = f;
}

void TextMatrixGlyph::do_glyph(Html *)
{
    m_context->state()->set_text_matrix(m_a, m_b, m_c, m_d, m_e, m_f);
}
