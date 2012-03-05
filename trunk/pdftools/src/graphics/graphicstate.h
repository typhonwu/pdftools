#pragma once
#ifndef GRAPHICSTATE_H
#define GRAPHICSTATE_H

class GraphicState {
private:

public:
    GraphicState();
    ~GraphicState();

    void push();
    void pop();
};

#endif
