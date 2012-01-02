#ifndef PARSER_H
#define PARSER_H

#include "token.h"

class Scanner;

class Parser {
private:
    Scanner *m_scanner;
    Token *token;
    bool m_valid;
    const char *version;

public:
    Parser();
    ~Parser();

    bool open_file(const char *path);
    void parse();
    bool is_valid();

private:
    void next_token();
    bool verify_version();
    long find_xref();
    bool match(TokenType type);
};

#endif
