#pragma once
#ifndef GRAPHICSTATE_H
#define GRAPHICSTATE_H

#include <vector>

using namespace std;

class Font;
class State;

class GraphicState {
private:
    Font *m_font;
    vector<State *> m_states;
    State *m_current_state;

public:
    GraphicState();
    ~GraphicState();

    void push();
    void pop();

    Font *font();
    void set_font(Font *font);
};

#endif
