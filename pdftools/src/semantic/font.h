#pragma once
#ifndef FONT_H
#define FONT_H

#include <string>

using namespace std;

class Font {
private:
    string m_name;
    string m_alias;
    
public:
    Font();
    ~Font();
    
    void set_name(string name);
    string name();
    void set_alias(string alias);
    string alias();
};

#endif
