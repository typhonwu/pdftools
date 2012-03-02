#include "html/html.h"
#include "glyph.h"

Glyph::Glyph()
{
    m_last_glyph = NULL;
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
    m_context = context;

    start_glyph(document);
    do_glyph(document);

    int size = m_childs.size();
    if (size > 0) {
        m_last_glyph = this;
    }
    for (int i = 0; i < size; i++) {
        m_childs[i]->set_last(m_last_glyph);
        m_childs[i]->execute(document, context);
        m_last_glyph = m_childs[i];
    }
    end_glyph(document);
}

void Glyph::set_last(Glyph *glyph)
{
    m_last_glyph = glyph;
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
