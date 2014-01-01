#pragma once

#include <string>

using namespace std;

class XML {
private:
    string m_buffer;
    
public:
    XML();
    ~XML();
    
    void start_document(const string&, const string& charset);
    void add_doctype(const string& name, const string& public_id, const string& sys_id);
    void add_attribute(const string& attribute, const string& value);
    void add_element(const string& value);
    void start_tag(const string& tag_name);
    void end_tag();
    void end_document();
    
    inline string content() const {
        return m_buffer;
    }
};
