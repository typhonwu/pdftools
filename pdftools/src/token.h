#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum TokenType {
    ENDFILE, ERROR, START_ARRAY, END_ARRAY, TRUE, FALSE,
    NAME, NUM, STRING, PERCENT, START_DICT, END_DICT, NEW_LINE,
    
    OBJ, END_OBJ, END_PDF, XREF, TRAILER, START_XREF, STREAM, END_STREAM,
    
    BT, ET, MP, DP, BMC, BDC, EMC, BX, EX, GS, TF, TC, TW, TZ, TL, TR, TS,
    TJ_UP, TJ_LO, T_AST, QUOTE, DOUBLE_QUOTE, TD_LO, TD_HI, TM, SCN, H, CM, L,
    G_LO, G_UP, W_AST, W_UP, W_LO, RE, CS, Q_UP, Q_LO, F_UP, F_LO, F_AST, M_LO,
    M_UP, I, K_LO, K_UP, J_LO, J_UP, D, RG_LO, RG_UP, DO, S_UP, S_LO, C,
    BI, ID, EI, V, Y, B_UP, B_UP_AST, B_LO, B_LO_AST
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
