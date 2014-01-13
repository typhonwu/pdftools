#include "page.h"
#include "context.h"
#include "document.h"
#include "../glyphs/glyph.h"
#include "../html/html.h"
#include <cstdlib>
#include <stdint.h>
#include <string.h>

Page::Page(Document *parent)
{
    m_media_box = NULL;
    m_crop_box = NULL;
    m_document = parent;
    m_id = 0;
    m_generation = 0;
    m_root = NULL;
}

Page::~Page()
{
    if (m_media_box) delete [] m_media_box;
    if (m_crop_box) delete [] m_crop_box;
    if (m_root) delete m_root;
}

void Page::set_root(Glyph *root)
{
	m_root = root;
}

void Page::add_fontmap(string alias, string font_name)
{
    m_fontmap[alias] = font_name;
}

string Page::font_name(string &alias)
{
    return m_fontmap[alias];
}

void Page::execute(Html *html)
{
	Context *context = new Context(m_document);
    context->set_current_page(this);
    m_root->execute(html, context);
    delete context;
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
