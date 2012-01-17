#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "rootnode.h"

class Scanner;
class TreeNode;

class Parser {
private:
    Scanner *m_scanner;
    Token *m_token;
    bool m_valid;
    bool m_linear;
    const wchar_t *m_version;

public:
    Parser();
    ~Parser();

    bool open_file(const char *path);
    RootNode *parse();
    bool is_valid();

private:
    void next_token();
    bool verify_version();
    bool match(TokenType type);
    
    void comment_sequence();
    TreeNode *object_sequence();
    TreeNode *xref_sequence();
    TreeNode *value_sequence();
};

#endif
