#include "state.h"

State::State()
{
    reset();
}

State::~State()
{
}

double State::get_text_font()
{
    return m_text_matrix[1][1];
}

void State::set_text_matrix(double a, double b, double c, double d, double e, double f)
{
    m_text_matrix[0][0] = a;
    m_text_matrix[0][1] = b;
    m_text_matrix[1][0] = c;
    m_text_matrix[1][1] = d;
    m_text_matrix[2][0] = e;
    m_text_matrix[2][1] = f;
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
