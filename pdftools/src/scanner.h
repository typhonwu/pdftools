#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <map>
#include "token.h"

class Scanner {
private:
    std::ifstream m_filein;
    std::map<const wchar_t *, TokenType> m_reserved;
    const wchar_t *m_error;
    
public:
    Scanner();
    ~Scanner();

    Token *next_token();
    bool open_file(const char *path);
    bool is_open();
    bool good();
    const wchar_t *error();
    
    void ignore_line();
    void find_last_xref();
    
private:
    void close_file();
    void unget_char();
    const wchar_t *get_line();
    bool is_space(const wchar_t c);
    wchar_t next_char();
    TokenType reserved_lookup(const wchar_t *s);
};

#endif
