#pragma once

#include <istream>
#include <stdint.h>
#include "token.h"

using namespace std;

class Scanner {
private:
    istream *m_filein;
    const char *m_error;
    Token m_current;
    bool m_charset_conversion;

public:
    Scanner();
    ~Scanner();

    Token *next_token();
    void set_istream(istream *stream);
    bool good();
    const char *error();

    void ignore_line();
    int ignore_stream(int length);
    int pos();
    void to_pos(int pos);
    char *get_stream(int length);
    char *get_image_stream();
    void disable_charset_conversion();

private:
    void unget_char();
    bool is_space(const char c);
    char next_char();
    TokenType reserved_lookup(const char *s);
};
