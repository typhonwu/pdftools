#include "font.h"

Font::Font()
{
}

Font::~Font()
{
}

void Font::set_name(string name)
{
    m_name = name;
}

string Font::name()
{
    return m_name;
}
