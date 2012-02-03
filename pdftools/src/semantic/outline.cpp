#include "outline.h"

Outline::Outline()
{
}

Outline::~Outline()
{
    vector<Outline *>::iterator i = m_childs.begin();
    while (i != m_childs.end()) {
        delete *i;
        i++;
    }
}

void Outline::set_title(string title)
{
    m_title = title;
}

void Outline::add_child(Outline *child)
{
    m_childs.push_back(child);
}

void Outline::set_destination(int id, int generation)
{
    m_id = id;
    m_generation = generation;
}
