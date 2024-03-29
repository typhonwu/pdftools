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
    
    void set_text_matrix(double a, double b, double c, double d, double e, double f);
    double get_text_font();

    Font *font();
    void set_font(Font *font);
};

#endif
