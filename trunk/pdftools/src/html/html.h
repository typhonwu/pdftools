#ifndef HTML_H
#define HTML_H

#include "xml/xml.h"

class Html {
private:
    XML m_xml;
    
public:
    Html();
    ~Html();
    
    void start_document();
    void start_header();
    void start_body();
    
    void end_document();
    void end_tag();
    
    void break_line();
    
    void add_paragraph();
    void add_element(const char *element);
    
    void set_title(const char *title);
    
    const char *content();
};

#endif
