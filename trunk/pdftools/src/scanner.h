#ifndef SCANNER_H
#define SCANNER_H

#include <istream>
#include <stdint.h>
#include <iconv.h>
#include "token.h"

using namespace std;

class Scanner {
private:
    istream *m_filein;
    const char *m_error;
    Token m_current;
    
public:
    Scanner();
    ~Scanner();

    Token *next_token();
    //bool open_file(const char *path);
    void set_istream(istream *stream);
    bool good();
    const char *error();
    
    void ignore_line();
    int pos();
    void to_pos(int pos);
    pair<int, int8_t *> get_stream(int length);
    
private:
    void unget_char();
    const char *get_line();
    bool is_space(const char c);
    char next_char();
    TokenType reserved_lookup(const char *s);
};

#endif
