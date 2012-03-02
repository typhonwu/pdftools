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
    // FIXME process font size
    if (m_document && m_page) {
        m_font = m_document->get_font(m_page->font_name(alias).c_str());
    }
}

Font *Context::font()
{
    return m_font;
}
