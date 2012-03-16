#include "graphicstate.h"
#include "state.h"
#include "semantic/context.h"
#include <iostream>

using namespace std;

GraphicState::GraphicState(Context *owner)
{
    m_context = owner;
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
    m_context->set_font_changed(true);
    m_current_state->set_text_matrix(a, b, c, d, e, f);
}

void GraphicState::push()
{
    //cout << "push" << endl;
}

void GraphicState::pop()
{
    //cout << "pop" << endl;
}

Font *GraphicState::font()
{
    return m_font;
}

void GraphicState::set_font(Font *font)
{
    m_context->set_font_changed(true);
    m_font = font;
}
