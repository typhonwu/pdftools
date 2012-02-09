#include "text.h"

Text::Text(string text)
{
    m_text = text;
}

void Text::execute(Html *document)
{
    document->add_element(m_text.c_str());
}
