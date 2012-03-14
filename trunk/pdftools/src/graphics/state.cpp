#include "state.h"

State::State()
{
    reset();
}

State::~State()
{
}

void State::reset()
{
    m_text_matrix[0][0] = 1;
    m_text_matrix[0][1] = 0;
    m_text_matrix[0][2] = 0;
    m_text_matrix[1][0] = 0;
    m_text_matrix[1][1] = 1;
    m_text_matrix[1][2] = 0;
    m_text_matrix[2][0] = 0;
    m_text_matrix[2][1] = 0;
    m_text_matrix[2][2] = 1;
}
