#pragma once
#ifndef STATE_H
#define STATE_H

class State {
private:
    int m_text_matrix[3][3];

public:
    State();
    ~State();

    void reset();
};

#endif
