#pragma once
#ifndef FONT_H
#define FONT_H

#include <string>

using namespace std;

class Font {
private:
    string m_name;
    
public:
    Font();
    ~Font();
    
    void set_name(string name);
    string name();
};

#endif
