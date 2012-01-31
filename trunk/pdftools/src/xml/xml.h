#ifndef XML_H
#define XML_H

#include <libxml/xmlwriter.h>

class XML {
private:
    xmlTextWriterPtr m_writer;
    xmlBufferPtr m_buffer;
    bool m_ended;
    
public:
    XML();
    ~XML();
    
    bool start_document(const char *version, const char *charset);
    bool add_doctype(const char *name, const char *public_id, const char *sys_id, const char *subset = NULL);
    bool add_attribute(const char *attribute, const char *value);
    bool add_element(const char *value);
    bool start_tag(const char *tag_name);
    bool end_tag();
    bool end_document();
    
    const char *content();
};

#endif
