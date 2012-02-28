#include "font.h"
#include "utils.h"

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

void Font::set_charmap_start(string value)
{
    m_charmap_start = value;
}

void Font::set_charmap_finish(string value)
{
    m_charmap_finish = value;
}

void Font::add_charmap(string character, string utf16value)
{
    m_charmap[character] = utf16be_to_utf8(utf16value);
}

string Font::translate(string &value)
{
    
}
