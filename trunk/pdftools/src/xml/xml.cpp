#include "xml.h"
#include <stdexcept>
#include <vector>

using namespace std;

#define CHECK_ERROR(x) if (x < 0) {return;}

XML::XML()
{
    m_buffer = xmlBufferCreate();
    if (!m_buffer) {
        throw runtime_error("Error creating XML Buffer");
    }
    m_writer = xmlNewTextWriterMemory(m_buffer, false);
    if (!m_writer) {
        throw runtime_error("Error creating XML Writter");
    }
    m_ended = false;
}

XML::~XML()
{
    if (!m_ended && m_writer) {
        xmlFreeTextWriter(m_writer);
    }
    if (m_buffer) xmlBufferFree(m_buffer);
}

const char *XML::content()
{
    if (m_buffer) {
        return (const char *)m_buffer->content;
    }
    return NULL;
}

bool XML::start_document(const char* version, const char* charset)
{
    if (m_writer) {
        return xmlTextWriterStartDocument(m_writer, version, charset, NULL) >= 0;
    }
    return false;
}

bool XML::add_doctype(const char *name, const char *public_id, const char *sys_id, const char *subset)
{
    if (m_writer) {
        return xmlTextWriterWriteDTD(m_writer, BAD_CAST name, BAD_CAST public_id, BAD_CAST sys_id, BAD_CAST subset);
    }
    return false;
}

bool XML::end_document()
{
    if (m_writer) {
        m_ended = true;
        xmlFreeTextWriter(m_writer);
    }
    return false;
}

bool XML::add_attribute(const char *attribute, const char *value)
{
    if (m_writer) {
        return xmlTextWriterWriteAttribute(m_writer, BAD_CAST attribute, BAD_CAST value) >= 0;
    }
    return false;
}

bool XML::add_element(const char *value)
{
    if (m_writer) {
        return xmlTextWriterWriteString(m_writer, BAD_CAST value) >= 0;
    }
    return false;
}

bool XML::start_tag(const char *tag_name)
{
    if (m_writer) {
        return xmlTextWriterStartElement(m_writer, BAD_CAST tag_name) >= 0;
    }
    return false;
}

bool XML::end_tag()
{
    if (m_writer) {
        return xmlTextWriterEndElement(m_writer) >= 0;
    }
    return false;
}
