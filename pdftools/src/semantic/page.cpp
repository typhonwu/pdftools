#include "page.h"
#include <cstdlib>
#include <stdint.h>
#include <string.h>

Page::Page()
{
    m_media_box = NULL;
    m_crop_box = NULL;
    m_id = 0;
    m_generation = 0;
}

Page::~Page()
{
    if (m_media_box) delete [] m_media_box;
    if (m_crop_box) delete [] m_crop_box;
}

void Page::add_glyph(Glyph *glyph)
{
    m_document.add_child(glyph);
}

void Page::execute(Html *document)
{
    m_document.execute(document);
}

void Page::set_link(char *link)
{
    m_link = link;
}

const char *Page::link()
{
    return m_link.c_str();
}

void Page::set_media_box(int a, int b, int c, int d)
{
    m_media_box = new int[4];
    m_media_box[0] = a;
    m_media_box[1] = b;
    m_media_box[2] = c;
    m_media_box[3] = d;
}

void Page::set_crop_box(int a, int b, int c, int d)
{
    m_crop_box = new int[4];
    m_crop_box[0] = a;
    m_crop_box[1] = b;
    m_crop_box[2] = c;
    m_crop_box[3] = d;

}

void Page::set_destination(int id, int generation)
{
    m_id = id;
    m_generation = generation;
}

int Page::id()
{
    return m_id;
}

int Page::generation()
{
    return m_generation;
}
