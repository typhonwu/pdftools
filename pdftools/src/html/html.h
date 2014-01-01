#pragma once
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
    void add_font(int size, bool bold, bool italic, bool fixed);
    void add_element(const string& element);
    
    void set_title(const string& title);
    void set_link(const string& rel, const string& type, const string& href);
    
    inline const string content() const {
        return m_xml.content();
    }
};

#endif
