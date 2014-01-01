#include "xml.h"
#include <stdexcept>
#include <vector>

using namespace std;

#define CHECK_ERROR(x) if (x < 0) {return;}

XML::XML()
{
}

XML::~XML()
{
}

void XML::start_document(const string& version, const string& charset) {
    m_buffer += "<?xml version=\"" + version + "\" encoding=\"" + charset + "\"?>";
}

void XML::add_doctype(const string& name, const string& public_id, const string& sys_id) {
    m_buffer += "<!DOCTYPE " + name;
    if (!public_id.empty()) {
        m_buffer += " PUBLIC \"" + public_id + "\"";
    }
    if (!sys_id.empty()) {
        m_buffer += "\"" + sys_id + "\"";
    }
    m_buffer += ">\n";
}

void XML::end_document() {
}

void XML::add_attribute(const string& attribute, const string& value) {
}

void XML::add_element(const string& value) {
}

void XML::start_tag(const string& tag_name) {
}

void XML::end_tag() {
}
