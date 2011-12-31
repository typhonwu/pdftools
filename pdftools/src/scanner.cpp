#include "scanner.h"
#include <fstream>

using namespace std;

#if HAVE_CONFIG_H
#include <config.h>
#endif

enum StateType {
    START, INNUM, INNAME, INBOOL, INSTRING, INHEXSTR, DONE
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
    m_filein.open(path, ios::binary);
    return is_open();
}

wchar_t Scanner::next_char()
{
    static bool new_char = false;

    wchar_t ret = NULL;
    if (!m_filein.good()) {
        ret = m_filein.get();
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

const wchar_t *Scanner::error()
{
    return m_error;
}

void Scanner::unget_char()
{
    m_filein.unget();
}

Token *Scanner::next_token()
{
    string token_string;
    TokenType current_token;
    StateType state = START;
    m_error = NULL;

    bool save;
    while (state != DONE) {
        wchar_t c = next_char();
        save = true;
        switch (state) {
        case START:
            if (isdigit(c) || (c == L'+') || (c == L'-') || (c == L'.')) {
                state = INNUM;
            } else if (c == L'%') {
                current_token = PERCENT;
                state = DONE;
            } else if (c == L'/') {
                save = false;
                state = INNAME;
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
            } else if ((c == L' ') || (c == L'\r') || (c == L'\n') || (c == L'\t') || (c == L'\f')) {
                save = false;
            } else if ((c == L't') || (c == L'f')) {
                save = false;
                state = INBOOL;
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
            token_string += (char) c;
        }
    }
    return new Token(current_token, token_string.c_str());
}