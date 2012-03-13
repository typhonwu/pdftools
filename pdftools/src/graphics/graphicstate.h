#pragma once
#ifndef GRAPHICSTATE_H
#define GRAPHICSTATE_H

class Font;

class GraphicState {
private:
    Font *m_font;

public:
    GraphicState();
    ~GraphicState();

    void push();
    void pop();

    Font *font();
    void set_font(Font *font);
};

#endif
