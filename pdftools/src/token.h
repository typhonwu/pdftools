#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    ENDFILE, ERROR, START_ARRAY, END_ARRAY,
    NAME, NUM, STRING, PERCENT, START_DICT, END_DICT,
    
    OBJ, END_OBJ, LINEARIZED, XREF, TREILER
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

    double to_number();
    std::wstring value();
    TokenType type();
};

#endif
