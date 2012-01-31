#include "pagelabel.h"

#include <iostream>

PageLabel::PageLabel(int start, int range, page_type type, string &name)
{
    m_start_number = start;
    m_range = range;
    m_type = type;
    m_name = name;
}

int PageLabel::start()
{
    return m_start_number;
}

int PageLabel::range()
{
    return m_range;
}

page_type PageLabel::type()
{
    return m_type;
}

string &PageLabel::name()
{
    return m_name;
}
