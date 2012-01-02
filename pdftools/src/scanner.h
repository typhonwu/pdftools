#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <map>
#include "token.h"

class Scanner {
private:
    std::ifstream m_filein;
    std::map<const wchar_t *, TokenType> reservedWords;
    const wchar_t *m_error;
    
public:
    Scanner();
    ~Scanner();

    Token *next_token();
    bool open_file(const char *path);
    bool is_open();
    const wchar_t *error();

private:
    void close_file();
    void unget_char();
    bool is_space(const wchar_t c);
    wchar_t next_char();
};

#endif
