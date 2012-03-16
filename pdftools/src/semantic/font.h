#pragma once
#ifndef FONT_H
#define FONT_H

#include <string>
#include <map>

using namespace std;

class Font {
private:
    bool m_italic;
    bool m_bold;
    bool m_fixed;
    double m_size;
    string m_name;
    string m_charmap_start;
    string m_charmap_finish;
    map<string, string> m_charmap;

public:
    Font();
    ~Font();

    void set_size(double size);
    void set_fixed(bool fixed);
    void set_name(string name);
    void set_italic(bool italic);
    string name();
    double size();
    bool italic();
    bool bold();
    bool fixed();

    void set_charmap_start(string value);
    void set_charmap_finish(string value);
    void add_charmap(string character, string utf16value);
    string translate(string &value);
};

#endif
