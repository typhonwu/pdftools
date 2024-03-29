#include "graphicstate.h"
#include "state.h"
#include <iostream>

using namespace std;

GraphicState::GraphicState()
{
    m_current_state = new State;
}

GraphicState::~GraphicState()
{
    if (m_current_state) delete m_current_state;
}

double GraphicState::get_text_font()
{
    return m_current_state->get_text_font();
}

void GraphicState::set_text_matrix(double a, double b, double c, double d, double e, double f)
{
    m_current_state->set_text_matrix(a, b, c, d, e, f);
}

void GraphicState::push()
{
}

void GraphicState::pop()
{
}

Font *GraphicState::font()
{
    return m_font;
}

void GraphicState::set_font(Font *font)
{
    m_font = font;
}
