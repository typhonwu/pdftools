#include "graphicstate.h"
#include <iostream>

using namespace std;

GraphicState::GraphicState()
{
}

GraphicState::~GraphicState()
{

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
    m_font = font;
}
