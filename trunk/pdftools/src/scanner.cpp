#include "scanner.h"
#include "utils.h"
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <iostream>

#if HAVE_CONFIG_H
#include <config.h>
#endif

using namespace std;

static const char *spaces = " \t\f";
static const char *special_chars = "\r\n<()/[]>";
static const char *numbers = "0123456789-+.";

enum StateType {
    START, INNUM, INNAME, INSTRING, INHEXSTR, DONE
};

struct reserved_words {
    TokenType type;
    const char *name;
};

static reserved_words words[] = { { OBJ, "obj" }, { END_OBJ, "endobj" }, {
        END_PDF, "EOF" }, { XREF, "xref" }, { TRUE, "true" },
        { FALSE, "false" }, { STREAM, "stream" }, { END_STREAM, "endstream" }, {
                START_XREF, "startxref" }, { TRAILER, "trailer" }, { BT, "BT" },
        { ET, "ET" }, { MP, "MP" }, { DP, "DP" }, { BMC, "BMC" },
        { BDC, "BDC" }, { EMC, "EMC" }, { BX, "BX" }, { EX, "EX" }, { TJ_UP,
                "TJ" }, { TJ_LO, "Tj" }, { QUOTE, "'" }, { DOUBLE_QUOTE, "\"" },
        { GS, "gs" }, { TF, "Tf" }, { TW, "Tw" }, { TZ, "Tz" }, { TL, "TL" }, {
                T_AST, "T*" }, { TR, "Tr" }, { TS, "Ts" }, { TC, "Tc" }, { TM,
                "Tm" }, { D0, "d0" }, { D1, "d1" }, { SH, "sh" }, { N, "n" }, {
                TD_LO, "Td" }, { TD_UP, "TD" }, { SCN_UP, "SCN" }, { SCN_LO,
                "scn" }, { SC_UP, "SC" }, { SC_LO, "sc" }, { G_LO, "g" }, {
                G_UP, "G" }, { RE, "re" }, { RI, "ri" }, { CS_UP, "CS" }, {
                CS_LO, "cs" }, { W_LO, "w" }, { W_UP, "W" }, { W_AST, "W*" }, {
                Q_UP, "Q" }, { Q_LO, "q" }, { F_AST, "f*" }, { F_UP, "F" }, {
                F_LO, "f" }, { RG_UP, "RG" }, { RG_LO, "rg" }, { M_LO, "m" }, {
                M_UP, "M" }, { K_LO, "k" }, { K_UP, "K" }, { J_LO, "j" }, {
                J_UP, "J" }, { S_LO, "s" }, { S_UP, "S" }, { C, "c" }, { CM,
                "cm" }, { DO, "Do" }, { L, "l" }, { D, "d" }, { H, "h" }, { V,
                "v" }, { Y, "y" }, { I, "i" }, { BI, "BI" }, { ID, "ID" }, {
                B_UP, "B" }, { B_UP_AST, "B*" }, { B_LO, "b" },
        { B_LO_AST, "b*" }, { EI, "EI" } };

inline unsigned int xtod(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return 0; // not a hex digit
}

Scanner::Scanner()
{
    m_error = NULL;
    m_charset_conversion = true;
}

Scanner::~Scanner()
{
}

void Scanner::disable_chatset_conversion()
{
    m_charset_conversion = false;
}

void Scanner::set_istream(istream *stream)
{
    m_filein = stream;
}

int Scanner::pos()
{
    return m_filein->tellg();
}

void Scanner::to_pos(int pos)
{
    m_filein->seekg(pos, ios::beg);
}

int Scanner::ignore_stream(int length)
{
    // Ignore first new line
    while (m_filein->good() && next_char() != '\n') {
    }
    //unget_char();
    int ret = m_filein->tellg();

    if (length > 0) {
        m_filein->ignore(length);
    } else {
        while (m_filein->good()) {
            int ret = m_filein->get();
            if ((ret == '\n' || ret == '\r') && m_filein->good()) {
                int pos = m_filein->tellg();
                int next = m_filein->get();
                // treat '\r\n', '\r' or '\n'
                if (next == 'e' || m_filein->get() == 'e') {
                    m_filein->unget();
                    Token *token = next_token();
                    if (token != NULL && token->type() == END_STREAM) {
                        // endstream: do not save the and char 
                        // and return the token start position
                        m_filein->seekg(pos);
                        break;
                    }
                }
                // not endstream
                m_filein->seekg(pos);
            }
        }
    }
    return ret;
}

char *Scanner::get_image_stream()
{
    // Ignore first new line
    while (m_filein->good() && next_char() != '\n') {
    }
    unget_char();

    while (m_filein->good()) {
        int ret = m_filein->get();
        if ((ret == '\n' || ret == '\r') && m_filein->good()) {
            int pos = m_filein->tellg();
            int next = m_filein->get();
            // treat '\r\n', '\r' or '\n'
            if (next == 'E' || m_filein->get() == 'I') {
                m_filein->unget();
                m_filein->unget();
                break;
            }
            // not endstream
            m_filein->seekg(pos);
        }
    }
    // FIXME save image stream
    return NULL;
}

char *Scanner::get_stream(int length)
{
    char *stream = new char[length];
    m_filein->read((char *) stream, length);
    return stream;
}

char Scanner::next_char()
{
    char ret = EOF;

    if (m_filein->good() && !m_filein->eof()) {
        ret = m_filein->get();
        if (ret == '\r') {
            char second = m_filein->get();
            if (second == '\n') {
                return '\n';
            }
            m_filein->unget();
        }
    }
    return ret;
}

bool Scanner::good()
{
    return m_filein->good();
}

void Scanner::ignore_line()
{
    while (next_char() != '\n')
        ;
    unget_char();
}

const char *Scanner::error()
{
    return m_error;
}

void Scanner::unget_char()
{
    m_filein->unget();
}

bool Scanner::is_space(const char c)
{
    return strchr(spaces, c) || (c == '\n') || (c == '\r') || (c == EOF);
}

TokenType Scanner::reserved_lookup(const char *s)
{
    int size = sizeof(words) / sizeof(reserved_words);
    for (int i = 0; i < size; i++) {
        if (!strcmp(words[i].name, s)) {
            return words[i].type;
        }
    }
    return NAME;
}

Token *Scanner::next_token()
{
    string token_string;
    TokenType current_token = ENDFILE;
    StateType state = START;
    int inner_string = 0;
    m_error = NULL;

    bool save;
    while (state != DONE && m_filein->good()) {
        char c = next_char();
        save = true;
        switch (state) {
        case START:
            if (strchr(numbers, c)) {
                state = INNUM;
            } else if (c == '%') {
                current_token = PERCENT;
                state = DONE;
            } else if (c == '[') {
                current_token = START_ARRAY;
                state = DONE;
            } else if (c == ']') {
                current_token = END_ARRAY;
                state = DONE;
            } else if (c == '>') {
                wchar_t next = next_char();
                if (next != '>') {
                    unget_char();
                    save = false;
                    current_token = ERROR;
                } else {
                    token_string += '>';
                    state = DONE;
                    current_token = END_DICT;
                }
                state = DONE;
            } else if (c == '(') {
                save = false;
                state = INSTRING;
            } else if (c == '<') {
                wchar_t next = next_char();
                if (next != '<') {
                    unget_char();
                    save = false;
                    state = INHEXSTR;
                } else {
                    token_string += '<';
                    state = DONE;
                    current_token = START_DICT;
                }
            } else if (is_space(c)) {
                save = false;
            } else if (isalpha(c) || c == '/' || c == '\'' || c == '"') {
                state = INNAME;
            } else if (c == '\n' || c == '\r') {
                state = DONE;
                current_token = NEW_LINE;
            } else if (c == EOF) {
                state = DONE;
                current_token = ENDFILE;
            } else {
                string msg = "Invalid char ";
                msg += c;
                error_message(msg.c_str());
                state = DONE;
                save = false;
                current_token = ERROR;
            }
            break;
        case INNUM:
            if (!isdigit(c) && (c != '.')) {
                /* backup in the input */
                unget_char();
                save = false;
                state = DONE;
                current_token = NUM;
            }
            break;
        case INHEXSTR:
            if (is_space(c)) {
                save = false;
            } else if (c == '>') {
                save = false;
                state = DONE;

                register unsigned int loop;
                string string;

                for (loop = 0; loop < token_string.length(); loop++) {
                    unsigned int h = xtod(token_string.at(loop)) << 4;
                    unsigned int l = 0;
                    loop++;
                    if (loop < token_string.length()) {
                        l = xtod(token_string.at(loop));
                    }
                    string.push_back(h + l);
                }
                if (m_charset_conversion) {
                    token_string = charset_to_utf8(string);
                } else {
                    token_string = string;
                }
                current_token = STRING;
            }
            break;
        case INSTRING:
            if (c == '(') {
                inner_string++;
            } else if (c == '\\') {
                // save the next char
                c = next_char();
                if (c == '\n' || c == '\r') {
                    save = false;
                }
            } else if (c == ')') {
                if (inner_string > 0) {
                    inner_string--;
                } else {
                    if (m_charset_conversion) {
                        token_string = charset_to_utf8(token_string);
                    }
                    save = false;
                    state = DONE;
                    current_token = STRING;
                }
            }
            break;
        case INNAME:
            if (is_space(c) || strchr(special_chars, c)) {
                save = false;
                unget_char();
                state = DONE;
                current_token = reserved_lookup(token_string.c_str());
            }
            break;
        case DONE:
            break;
        default:
            /* should never happen */
            error_message("Invalid scanner state");
            state = DONE;
            current_token = ERROR;
            break;
        }
        if (save) {
            token_string.push_back(c);
        }
    }
    m_current.set_type(current_token);
    m_current.set_value(token_string);

    return &m_current;
}
