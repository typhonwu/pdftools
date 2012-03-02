#pragma once
#ifndef OUTLINE_H
#define OUTLINE_H

#include <vector>
#include <string>

using namespace std;

class Outline {
private:
    vector<Outline *> m_childs;
    string m_title;
    int m_id;
    int m_generation;
    
public:
    Outline();
    ~Outline();
    
    void add_child(Outline *child);
    void set_destination(int id, int generation);
    void set_title(string title);
    
    int id();
    int generation();
    const char *title();
    
    int size();
    Outline *child(int index);
};

#endif
