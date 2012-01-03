#include "scanner.h"
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

enum StateType {
    START, INNUM, INNAME, INSTRING, INHEXSTR, DONE
};

Scanner::Scanner() : m_error(NULL)
{
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

wchar_t Scanner::next_char()
{
    static bool new_char = false;
    wchar_t ret = NULL;
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
            } else if (c == L'/') {
                save = true;
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
                    // TODO Array
                }
            } else if (is_space(c)) {
                save = false;
            } else if (iswalpha(c)) {
                save = true;
                state = INNAME;
            } else {
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
        case INSTRING:
            if (c == '"') {
                save = false;
                state = DONE;
                current_token = STRING;
            }
            break;
        case INNAME:
            if (is_space(c) || c == L'<' || c == L'\\') {
                save = false;
                state = DONE;
                current_token = NAME;
            }
            break;
        case DONE:
            break;
        default:
            /* should never happen */
            m_error = L"Invalid scanner state";
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

void Scanner::find_last_xref()
{
    register long xref = 0;
    const wchar_t *_line;

    while (m_filein.good()) {
        _line = get_line();

        if (!wcscmp(_line, L"startxref")) {
            _line = get_line();
            wstringstream(_line) >> xref;
#ifdef DEBUG
            wcout << L"XRef: " << xref << L" in pos " << _line << endl;
#endif
        }
    }

}
