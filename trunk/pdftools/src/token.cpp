#include "token.h"
#include <cstring>

using namespace std;

Token::Token(TokenType type, string value)
{
    m_type = type;
    m_value = value;
}

string Token::value()
{
    return m_value;
}

TokenType Token::type()
{
    return m_type;
}
