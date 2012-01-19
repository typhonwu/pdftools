#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <ios>
#include <map>
#include <vector>
#include <stdint.h>
#include "token.h"

class Scanner {
private:
    std::ifstream m_filein;
    std::map<const char *, TokenType> m_reserved;
    const char *m_error;
    
public:
    Scanner();
    ~Scanner();

    Token *next_token();
    bool open_file(const char *path);
    bool is_open();
    bool good();
    const char *error();
    
    void ignore_line();
    int pos();
    void to_pos(int pos);
    std::vector<uint8_t> get_stream();
    
private:
    void close_file();
    void unget_char();
    const char *get_line();
    bool is_space(const char c);
    char next_char();
    TokenType reserved_lookup(const char *s);
};

#endif
