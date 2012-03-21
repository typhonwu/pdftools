#include "context.h"
#include "document.h"
#include "page.h"
#include "font.h"
#include <cstdlib>

Context::Context(Document *document)
{
    m_document = document;
    m_page = NULL;
    m_font = NULL;
    m_font_changed = false;
    m_use_font = false;
    m_font_size = 1;
}

Context::~Context()
{
}

void Context::set_font_size(double size)
{
    m_font_size = size;
}

double Context::font_size()
{
    return m_font_size;
}

void Context::set_font_changed(bool changed)
{
    m_font_changed = changed;
}

bool Context::font_changed()
{
    return m_font_changed;
}

bool Context::use_font()
{
    return m_use_font;
}

void Context::set_use_font(bool use_font)
{
    m_use_font = use_font;
}

void Context::set_current_page(Page *page)
{
    m_page = page;
}

Document *Context::document()
{
    return m_document;
}

void Context::set_current_font(string &alias, int size)
{
    if (m_document && m_page) {
        m_font_changed = true;
        m_font = m_document->get_font(m_page->font_name(alias).c_str());
        m_font->set_size(size);
    }
}

Font *Context::font()
{
    return m_font;
}
