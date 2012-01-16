#include "scanner.h"
#include "utils.h"
#include <fstream>
#include <locale>
#include <sstream>
#include <cstring>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

inline unsigned int xtod(wchar_t c)
{
    if (c >= L'0' && c <= L'9') return c - L'0';
    if (c >= L'A' && c <= L'F') return c - L'A' + 10;
    if (c >= L'a' && c <= L'f') return c - L'a' + 10;
    return 0; // not Hex digit
}

enum StateType {
    START, INNUM, INNAME, INSTRING, INHEXSTR, DONE
};

Scanner::Scanner() : m_error(NULL)
{
    m_reserved[L"obj"] = OBJ;
    m_reserved[L"endobj"] = END_OBJ;
    m_reserved[L"EOF"] = END_PDF;
    m_reserved[L"xref"] = XREF;
    m_reserved[L"startxref"] = START_XREF;
    m_reserved[L"trailer"] = TRAILER;
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

wchar_t Scanner::next_char()
{
    static bool new_char = false;
    wchar_t ret = L'\0';
    locale loc;

    if (m_filein.good()) {
        ret = use_facet < ctype<wchar_t> >(loc).widen((char) m_filein.get());
        if (new_char) {
            if (ret == L'\n' || ret == L'\r') {
                new_char = false;
                return next_char();
            }
        }
        if (ret == L'\n') {
            new_char = true;
        } else if (ret == L'\r') {
            new_char = true;
            ret = L'\n';
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

const wchar_t *Scanner::error()
{
    return m_error;
}

void Scanner::unget_char()
{
    m_filein.unget();
}

bool Scanner::is_space(const wchar_t c)
{
    return (c == L' ') || (c == L'\r') || (c == L'\n') || (c == L'\t') || (c == L'\f');
}

TokenType Scanner::reserved_lookup(const wchar_t *s)
{
    map<const wchar_t *, TokenType>::iterator i;
    for (i = m_reserved.begin(); i != m_reserved.end(); i++) {
        if (!wcscmp(s, (*i).first)) {
            return (*i).second;
        }
    }
    return NAME;
}

Token *Scanner::next_token()
{
    wstring token_string;
    TokenType current_token;
    StateType state = START;
    m_error = NULL;

    bool save;
    while (state != DONE) {
        wchar_t c = next_char();
        if (!c) {
            return NULL;
        }
        save = true;
        switch (state) {
        case START:
            if (isdigit(c) || (c == L'+') || (c == L'-') || (c == L'.')) {
                state = INNUM;
            } else if (c == L'%') {
                current_token = PERCENT;
                state = DONE;
            } else if (c == L'[') {
                current_token = START_ARRAY;
                state = DONE;
            } else if (c == L']') {
                current_token = END_ARRAY;
                state = DONE;
            } else if (c == L'>') {
                wchar_t next = next_char();
                if (next != L'>') {
                    unget_char();
                    save = false;
                    current_token = ERROR;
                } else {
                    token_string += L'>';
                    state = DONE;
                    current_token = END_DICT;
                }
                state = DONE;
            } else if (c == L'(') {
                save = false;
                state = INSTRING;
            } else if (c == L'<') {
                wchar_t next = next_char();
                if (next != L'<') {
                    unget_char();
                    save = false;
                    state = INHEXSTR;
                } else {
                    token_string += L'<';
                    state = DONE;
                    current_token = START_DICT;
                }
            } else if (is_space(c)) {
                save = false;
            } else if (iswalpha(c) || c == L'/') {
                state = INNAME;
            } else {
                wstring msg = L"Invalid char ";
                msg += c;
                error_message(msg.c_str());
                state = DONE;
                save = false;
                current_token = ERROR;
            }
            break;
        case INNUM:
            if (!isdigit(c)) {
                /* backup in the input */
                unget_char();
                save = false;
                state = DONE;
                current_token = NUM;
            }
            break;
        case INHEXSTR:
            if (c == '>') {
                save = false;
                state = DONE;

                unsigned int loop;
                wstring string;

                for (loop = 0; loop < token_string.length(); loop++) {
                    unsigned int h = xtod(token_string.at(loop)) << 4;
                    unsigned int l = 0;
                    loop++;
                    if (loop < token_string.length()) {
                        l = xtod(token_string.at(loop));
                    }
                    string += (wchar_t)(h + l);
                }
                token_string = string;
                current_token = STRING;
            }
            break;
        case INSTRING:
            if (c == '"') {
                save = false;
                state = DONE;
                current_token = STRING;
            }
            break;
        case INNAME:
            if (is_space(c) || c == L'<' || c == L'\\' || c == L'//' || c == L'[') {
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
            error_message(L"Invalid scanner state");
            state = DONE;
            current_token = ERROR;
            break;
        }
        if (save) {
            token_string += c;
        }
    }
    return new Token(current_token, token_string.c_str());
}

const wchar_t *Scanner::get_line()
{
    wstring _buffer;
    register bool starting = true;

    while (m_filein.good()) {
        wchar_t c = next_char();
        if ((c == L'\n' || c == L'\r')) {
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
