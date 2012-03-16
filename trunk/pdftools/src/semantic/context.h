#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include "graphics/graphicstate.h"

using namespace std;

class Document;
class Font;
class Page;

class Context {
private:
    Document *m_document;
    Page *m_page;
    Font *m_font;
    GraphicState *m_state;
    bool m_font_changed;
    bool m_use_font;

public:
    Context(Document *document);
    ~Context();

    void set_use_font(bool use_font);
    void set_font_changed(bool changed);
    void set_current_font(string &alias, int size);
    void set_current_page(Page *page);

    bool use_font();
    bool font_changed();
    Font *font();
    Document *document();
    GraphicState *state();
};

#endif
