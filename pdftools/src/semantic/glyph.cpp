#include "glyph.h"

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

void Glyph::execute(Html *document)
{
    start_glyph(document);
    do_glyph(document);

    for (int i = 0; i < m_childs.size(); i++) {
        m_childs[i]->execute(document);
    }

    end_glyph(document);
}

void Glyph::do_glyph(Html *document)
{
}

void Glyph::start_glyph(Html *document)
{
}

void Glyph::end_glyph(Html *document)
{
}
