#pragma once
#ifndef FONT_H
#define FONT_H

#include <string>
#include <map>

using namespace std;

class Font {
private:
    string m_name;
    string m_charmap_start;
    string m_charmap_finish;
    map<string, string> m_charmap;
    
public:
    Font();
    ~Font();
    
    void set_name(string name);
    string name();
    
    void set_charmap_start(string value);
    void set_charmap_finish(string value);
    void add_charmap(string character, string utf16value);
    string translate(string &value);
};

#endif
