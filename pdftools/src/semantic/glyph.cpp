#include "glyph.h"
#include "context.h"

Glyph::Glyph()
{
}

Glyph::~Glyph()
{
    vector<Glyph *>::iterator i;
    for (i = m_childs.begin(); i != m_childs.end(); i++) {
        delete *i;
    }
}

void Glyph::add_child(Glyph *glyph)
{
    if (glyph) {
        m_childs.push_back(glyph);
    }
}

void Glyph::execute(Html *document, Context *context)
{
    start_glyph(document, context);
    do_glyph(document, context);

    int size = m_childs.size();
    for (int i = 0; i < size; i++) {
        m_childs[i]->execute(document, context);
    }
    end_glyph(document, context);
}

void Glyph::do_glyph(Html *document, Context *context)
{
}

void Glyph::start_glyph(Html *document, Context *context)
{
}

void Glyph::end_glyph(Html *document, Context *context)
{
}
