#include "font.h"
#include "../utils.h"
#include <cstring>

Font::Font()
{
    m_italic = false;
    m_bold = false;
    m_fixed = false;
    m_size = 0;
}

Font::~Font()
{
}

void Font::set_size(double size)
{
    m_size = size;
}

double Font::size()
{
    return m_size;
}

bool Font::fixed()
{
    return m_fixed;
}

void Font::set_fixed(bool fixed)
{
    m_fixed = fixed;
}

void Font::set_italic(bool italic)
{
    m_italic = italic;
}

bool Font::italic()
{
    return m_italic;
}

bool Font::bold()
{
    return m_bold;
}

void Font::set_name(string name)
{
    if (name.find("Bold") != string::npos) {
        m_bold = true;
    }
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
    string ret;
    int size = m_charmap_start.size();

    if (m_charmap.size() == 0) {
        return value;
    }

    const char *start = m_charmap_start.c_str();
    const char *finish = m_charmap_finish.c_str();

    const char *buffer = value.c_str();
    int length = value.size();
    for (int loop = 0; loop < length; loop++, buffer += size) {
        register int f = memcmp(start, buffer, m_charmap_start.length());
        register int s = memcmp(finish, buffer, m_charmap_finish.length());
        if (f <= 0 && s >= 0) {
            string c = string(buffer, size);
            ret += m_charmap[c];
        } else {
            ret += *buffer;
        }
    }
    return ret;
}
