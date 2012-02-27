#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include "scanner.h"
#include "token.h"
#include "nodes/nodes.h"
#include <istream>

using namespace std;

class PageParser {
private:
    Scanner m_scanner;
    Token *m_token;
    RootNode *m_root;

public:
    PageParser(istream *stream);
    ~PageParser();
    
    RootNode *parse();
    
private:
    void next_token();
    bool match(TokenType type);
    
    TreeNode *value_sequence();
    TreeNode *bi_sequence();
    TreeNode *bdc_sequence(vector<TreeNode *> &values);
    TreeNode *text_sequence(vector<TreeNode *> &values);
    void tjup_sequence(RootNode *root, vector<TreeNode *> &values);
};

#endif