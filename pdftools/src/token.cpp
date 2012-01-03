#include "token.h"
#include <cstring>

using namespace std;

Token::Token(TokenType type, wstring value)
{
    m_type = type;
    m_value = value;
}

wstring Token::value()
{
    return m_value;
}

TokenType Token::type()
{
    return m_type;
}
