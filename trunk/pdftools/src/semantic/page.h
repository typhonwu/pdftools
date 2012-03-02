#pragma once
#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <map>

using namespace std;

class Context;
class Document;
class Glyph;
class Html;

class Page {
private:
    int *m_media_box;
    int *m_crop_box;
    int m_id;
    int m_generation;
    string m_link;
    Glyph *m_root;
    Document *m_document;
    map<string, string> m_fontmap;

public:
    Page(Document *parent);
    ~Page();

    void set_media_box(int a, int b, int c, int d);
    void set_crop_box(int a, int b, int c, int d);
    void set_destination(int id, int generation);
    void set_root(Glyph *root);
    int id();
    int generation();
    void set_link(char *link);
    const char *link();

    void execute(Html *html);
    
    void add_fontmap(string alias, string font_name);
    string font_name(string &alias);
};

#endif
