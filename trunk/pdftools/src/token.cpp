#include "token.h"
#include <cstring>

using namespace std;

Token::Token()
{
    m_type = ENDFILE;
}
void Token::set_value(string value)
{
    m_value = value;
}

void Token::set_type(TokenType type)
{
    m_type = type;
}

double Token::to_number()
{
    bool in_integer = true;
    bool negative = false;
    double decimals = 1;
    double value = 0;
    string::iterator it = m_value.begin();

    while (it != m_value.end()) {
        if ((*it) >= '0' && (*it) <= '9') {
            if (in_integer) {
                value *= 10;
                value += (*it) - '0';
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

string Token::value()
{
    return m_value;
}

TokenType Token::type()
{
    return m_type;
}
