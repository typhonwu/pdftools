#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    ENDFILE, ERROR,
    NAME, NUM, STRING, PERCENT
};

struct ReservedWords {
    const char *word;
    TokenType token;
};

class Token {
private:
    TokenType m_type;
    std::string m_value;

public:
    Token(TokenType type, std::string value);

    std::string value();
    TokenType type();
};

#endif
