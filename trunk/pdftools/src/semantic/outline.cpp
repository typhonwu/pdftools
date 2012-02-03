#include "outline.h"

Outline::Outline()
{
    m_id = 0;
    m_generation = 0;
}

Outline::~Outline()
{
    vector<Outline *>::iterator i = m_childs.begin();
    while (i != m_childs.end()) {
        delete *i;
        i++;
    }
}

const char *Outline::title()
{
    return m_title.c_str();
}

int Outline::size()
{
    return m_childs.size();
}

Outline *Outline::child(int index)
{
    return m_childs[index];
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

int Outline::id()
{
    return m_id;
}

int Outline::generation()
{
    return m_generation;
}
