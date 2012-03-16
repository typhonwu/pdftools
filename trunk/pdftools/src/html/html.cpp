#include "html.h"
#include <sstream>

using namespace std;

Html::Html()
{
    m_xml.start_document("1.0", "UTF-8");
    m_xml.add_doctype("html", "-//W3C//DTD XHTML 1.1//EN", "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd");
}

Html::~Html()
{
}

void Html::break_line()
{
    m_xml.start_tag("br");
    m_xml.end_tag();
}

void Html::add_paragraph()
{
    m_xml.start_tag("p");
}

void Html::start_header()
{
    m_xml.start_tag("head");
}

void Html::start_body()
{
    m_xml.start_tag("body");
}

void Html::set_title(const char* title)
{
    m_xml.start_tag("title");
    m_xml.add_element(title);
    m_xml.end_tag();
}

void Html::add_font(int size, bool bold, bool italic, bool fixed)
{
    stringstream style;
    style << "font-size:" << size << "pt";

    if (italic) {
        style << ";font-style:italic";
    } else {
        style << ";font-style:normal";
    }
    if (bold) {
        style << ";font-weight:bold";
    } else {
        style << ";font-weight:normal";
    }
    if (fixed) {
        style << ";font-family:courier,\"Courier New\",monospace";
    }

    m_xml.start_tag("span");
    m_xml.add_attribute("style", style.str().c_str());
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
