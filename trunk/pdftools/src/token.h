#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    ENDFILE, ERROR,
    NAME, NUM, STRING, PERCENT, START_ARRAY, END_ARRAY
};

struct ReservedWords {
    const char *word;
    TokenType token;
};

class Token {
private:
    TokenType m_type;
    std::wstring m_value;

public:
    Token(TokenType type, std::wstring value);

    std::wstring value();
    TokenType type();
};

#endif
