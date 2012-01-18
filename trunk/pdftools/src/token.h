#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    ENDFILE, ERROR, START_ARRAY, END_ARRAY,
    NAME, NUM, STRING, PERCENT, START_DICT, END_DICT,
    
    OBJ, END_OBJ, END_PDF, XREF, TRAILER, START_XREF, STREAM, END_STREAM
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

    double to_number();
    std::string value();
    TokenType type();
};

#endif
