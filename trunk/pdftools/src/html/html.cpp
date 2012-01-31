#include "html.h"

Html::Html()
{
    m_xml.start_document("1.0", "UTF-8");
    m_xml.add_doctype("html", "-//W3C//DTD XHTML 1.1//EN", "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd");
}

Html::~Html()
{
}

void Html::start_header()
{
    m_xml.start_tag("header");
}

void Html::start_body()
{
    m_xml.start_tag("body");
//    
//    m_xml.start_tag("div");
//    add_element("teste ");
//    m_xml.start_tag("b");
//    add_element("negrito");
//    end_tag();
//    end_tag();
}

void Html::set_title(const char* title)
{
    m_xml.start_tag("title");
    add_element(title);
    end_tag();
}

void Html::add_element(const char *value)
{
    m_xml.add_element(value);
}

void Html::end_tag()
{
    m_xml.end_tag();
}

void Html::start_document()
{
    m_xml.start_tag("html");
    m_xml.add_attribute("xmlns", "http://www.w3.org/1999/xhtml");
}

void Html::end_document()
{
    end_tag();
    m_xml.end_document();
}

const char *Html::content()
{
    return m_xml.content();
}
