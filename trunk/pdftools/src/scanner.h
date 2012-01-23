#ifndef SCANNER_H
#define SCANNER_H

#include <istream>
#include <ios>
#include <map>
#include <vector>
#include <stdint.h>
#include "token.h"

using namespace std;

class Scanner {
private:
    istream *m_filein;
    map<const char *, TokenType> m_reserved;
    const char *m_error;
    
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
    vector<int8_t> get_stream();
    
private:
    void unget_char();
    const char *get_line();
    bool is_space(const char c);
    char next_char();
    TokenType reserved_lookup(const char *s);
};

#endif
