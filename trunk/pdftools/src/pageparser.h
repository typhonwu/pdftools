#pragma once
#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include "scanner.h"
#include "genericparser.h"
#include <istream>
#include <vector>

using namespace std;

class BDCNode;
class RootNode;
class TreeNode;
class Token;

class PageParser: public GenericParser {
private:
    RootNode *m_root;

public:
    PageParser(istream *stream);
    ~PageParser();

    RootNode *parse();

private:
    TreeNode *font_sequence(vector<TreeNode *> &values);
    TreeNode *bi_sequence();
    BDCNode *bdc_sequence(vector<TreeNode *> &values, RootNode *parent);
    TreeNode *text_sequence(vector<TreeNode *> &values);
    void tjup_sequence(RootNode *root, vector<TreeNode *> &values);

};

#endif
