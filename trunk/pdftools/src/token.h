#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum TokenType {
    ENDFILE, ERROR, START_ARRAY, END_ARRAY, TRUE, FALSE,
    NAME, NUM, STRING, PERCENT, START_DICT, END_DICT, NEW_LINE,
    
    OBJ, END_OBJ, END_PDF, XREF, TRAILER, START_XREF, STREAM, END_STREAM,
    
    BT, ET, MP, DP, BMC, BDC, EMC, BX, EX, GS, TF, TC, TW, TZ, TL, TR, TS,
    TJ_UP, TJ_LO, TJ_AST, QUOTE, DOUBLE_QUOTE, TD_LO, TD_HI, TM, SCN
};

struct ReservedWords {
    const char *word;
    TokenType token;
};

class Token {
private:
    TokenType m_type;
    string m_value;

public:
    Token();

    void set_value(string value);
    void set_type(TokenType type);
    double to_number();
    string value();
    TokenType type();
};

#endif
