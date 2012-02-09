#ifndef ANALYSE_H
#define ANALYSE_H

#include "semantic/document.h"
#include "nodes/nodes.h"
#include <vector>
#include <map>
#include <sstream>
#include <string>

using namespace std;

class Outline;

class Analyze {
private:
    Document *m_document;
    RootNode *m_tree;
    TreeNode *m_page_tree;
    map<string, TreeNode *> m_names;

public:
    Analyze();
    ~Analyze();

    Document *analyze_tree(RootNode *tree);

private:
    void analyze_xref();
    void analyze_info();
    void analyze_root();
    void analyze_names(MapNode *values);
    void analyze_outlines(MapNode *values, Outline *parent = NULL);
    void analyze_pages(TreeNode *page, ArrayNode *mediabox = NULL);

    Page *process_page(int id, int generation, stringstream &stream_value, ArrayNode * mediabox);

    string get_string_value(TreeNode *value);
    double get_number_value(TreeNode *value, int default_value = 0);

    ObjNode *get_object(int id, int generation);
    ObjNode *get_object(RefNode *ref);
    TreeNode *get_real_value(TreeNode *value);
    TreeNode *get_real_obj_value(TreeNode *value);
    TreeNode *get_named_value(string name);
    void get_stream(ArrayNode *array, stringstream &stream_value);
};

#endif
