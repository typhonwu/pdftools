#include "token.h"
#include <cstring>

using namespace std;

Token::Token(TokenType type, wstring value)
{
    m_type = type;
    m_value = value;
}

double Token::to_number()
{
    bool in_integer = true;
    bool negative = false;
    double decimals = 1;
    double value = 0;
    wstring::iterator it = m_value.begin();

    while (it != m_value.end()) {
        if ((*it) >= L'0' && (*it) <= L'9') {
            if (in_integer) {
                value *= 10;
                value += (*it) - L'0';
            } else {
                decimals /= 10;
                value += decimals * (*it);
            }
        } else if (*it == '.') {
            in_integer = false;
        } else if (*it == '-') {
            negative = true;
        }
        it++;
    }
    if (negative) {
        value *= -1;
    }
    return value;
}

wstring Token::value()
{
    return m_value;
}

TokenType Token::type()
{
    return m_type;
}
