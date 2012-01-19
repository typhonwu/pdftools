#ifndef ANALYSE_H
#define ANALYSE_H

#include "semantic/document.h"
#include "nodes/nodes.h"

class Analyze {
private:
    Document *m_document;
    RootNode *m_tree;
    TreeNode *m_pages;

public:
    Analyze();
    ~Analyze();

    Document *analyze_tree(RootNode *tree);

private:
    void analyze_xref();
    void analyze_info();
    void analyse_root();
    void analyse_pages();
    string get_string_value(TreeNode *value);
    ObjNode *get_object(int id, int generation);
    ObjNode *get_object(RefNode *ref);
    TreeNode *get_real_value(TreeNode *value);
};

#endif
