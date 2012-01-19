#include "scanner.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <vector>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

static const char *spaces = " \r\t\f";
static const char *special_chars = "<()/[]>";
static const char *numbers = "0123456789-+.";

enum StateType {
    START, INNUM, INNAME, INSTRING, INHEXSTR, DONE
};

inline unsigned int xtod(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0; // not Hex digit
}

Scanner::Scanner() : m_error(NULL)
{
    m_reserved["obj"] = OBJ;
    m_reserved["endobj"] = END_OBJ;
    m_reserved["EOF"] = END_PDF;
    m_reserved["xref"] = XREF;
    m_reserved["stream"] = STREAM;
    m_reserved["endstream"] = END_STREAM;
    m_reserved["startxref"] = START_XREF;
    m_reserved["trailer"] = TRAILER;
}

Scanner::~Scanner()
{
    close_file();
}

void Scanner::close_file()
{
    if (is_open()) {
        m_filein.close();
    }
}

bool Scanner::is_open()
{
    return m_filein.is_open();
}

bool Scanner::open_file(const char *path)
{
    close_file();
    m_filein.open(path);
    return is_open();
}

int Scanner::pos()
{
    return m_filein.tellg();
}

void Scanner::to_pos(int pos)
{
    m_filein.seekg(pos);
}

vector<uint8_t> Scanner::get_stream()
{
    vector<uint8_t> stream;

    // Ignore first new line
    while (m_filein.good() && m_filein.get() != '\n');

    while (m_filein.good()) {
        uint8_t ret = m_filein.get();
        if ((ret == '\n' || ret == '\r') && m_filein.good()) {
            int pos = m_filein.tellg();
            int next = m_filein.get();
            if (next == 'e') {
                m_filein.unget();
                Token *token = next_token();
                if (token != NULL && token->type() == END_STREAM) {
                    // endstream: do not save the and char 
                    // and return the token start position
                    m_filein.seekg(pos);
                    delete token;
                    break;
                }
                delete token;
            }
            // not endstream
            m_filein.seekg(pos);
        }
        stream.push_back(ret);
    }
    return stream;
}

char Scanner::next_char()
{
    char ret = EOF;

    if (m_filein.good() && !m_filein.eof()) {
        ret = m_filein.get();
        if (ret == '\r') {
            char second = m_filein.get();
            if (second == '\n') {
                return '\n';
            }
            m_filein.unget();
        }
    }
    return ret;
}

bool Scanner::good()
{
    return m_filein.good();
}

void Scanner::ignore_line()
{
    get_line();
}

const char *Scanner::error()
{
    return m_error;
}

void Scanner::unget_char()
{
    m_filein.unget();
}

bool Scanner::is_space(const char c)
{
    return strchr(spaces, c) || (c == '\n') || (c == EOF);
}

TokenType Scanner::reserved_lookup(const char *s)
{
    map<const char *, TokenType>::iterator i;
    for (i = m_reserved.begin(); i != m_reserved.end(); i++) {
        if (!strcmp(s, (*i).first)) {
            return (*i).second;
        }
    }
    return NAME;
}

Token *Scanner::next_token()
{
    string token_string;
    TokenType current_token = ENDFILE;
    StateType state = START;
    m_error = NULL;

    bool save;
    while (state != DONE && m_filein.good()) {
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
            } else if (isalpha(c) || c == '/') {
                state = INNAME;
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

                unsigned int loop;
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
                token_string = string;
                current_token = STRING;
            }
            break;
        case INSTRING:
            if (c == '\\') {
                // save the next char
                c = next_char();
            } else if (c == ')') {
                save = false;
                state = DONE;
                current_token = STRING;
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
    return new Token(current_token, token_string);
}

const char *Scanner::get_line()
{
    string _buffer;
    register bool starting = true;

    while (m_filein.good()) {
        wchar_t c = next_char();
        if ((c == '\n' || c == '\r')) {
            if (starting) {
                continue;
            } else {
                break;
            }
        }
        _buffer += c;
        starting = false;
    }
    return _buffer.c_str();
}
