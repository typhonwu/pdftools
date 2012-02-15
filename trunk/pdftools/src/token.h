#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum TokenType {
    ENDFILE, ERROR, START_ARRAY, END_ARRAY, TRUE, FALSE,
    NAME, NUM, STRING, PERCENT, START_DICT, END_DICT, NEW_LINE,

    OBJ, END_OBJ, END_PDF, XREF, TRAILER, START_XREF, STREAM, END_STREAM,

    // Graphics state
    W_LO, J_LO, J_UP, M_UP, D, RI, I, GS,

    // Path painting 
    S_UP, S_LO, F_UP, F_LO, F_AST, B_UP, B_UP_AST, B_LO, B_LO_AST, N,
    
    // Special Graphics state
    Q_UP, Q_LO, CM, V, Y,

    // Path construction 
    M_LO, L, C, H, RE,

    // Clipping paths 
    W_AST, W_UP,

    // Text objects 
    BT, ET,

    // Text state 
    TC, TW, TZ, TL, TF, TR, TS,

    // Text positioning 
    TD_UP, TD_LO, TM, T_AST,

    // Text showing 
    TJ_UP, TJ_LO, QUOTE, DOUBLE_QUOTE,

    // Type 3 fonts 
    D0, D1,

    // Color
    CS_UP, CS_LO, SCN_UP, SCN_LO, SC_UP, SC_LO, G_LO, G_UP, RG_LO, RG_UP, K_LO, K_UP, 
 
    // Shading patterns 
    SH,

    // Inline images 
    BI, ID, EI,
    
    // XObjects 
    DO,
    
    // Marked content 
    MP, DP, BMC, BDC, EMC,
    
    // Compatibility 
    BX, EX
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
