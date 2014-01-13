#pragma once

#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

class Scanner;
class Outline;
class Document;
class RootNode;
class TreeNode;
class MapNode;
class ArrayNode;
class ObjNode;
class RefNode;
class Font;
class Page;

class Analyze {
private:
    Document *m_document;
    RootNode *m_tree;
    Scanner *m_scanner;
    TreeNode *m_page_tree;
    const string m_filein;
    map<string, TreeNode *> m_names;

public:
    Analyze(const string& filein);
    ~Analyze();

    Document *analyze_tree(RootNode *tree);

private:
    void analyze_xref();
    void analyze_info();
    void analyze_root();
    void analyze_names(MapNode *values);
    void analyze_outlines(MapNode *values, Outline *parent = NULL);
    void analyze_outline(ArrayNode *values, Outline *outline);
    void analyze_pages(TreeNode *page, ArrayNode *mediabox = NULL);
    Font *analyze_font(MapNode *fontmap);

    Page *process_page(int id, int generation, stringstream *stream_value, MapNode *catalog, ArrayNode * mediabox);

    string get_string_value(TreeNode *value);
    double get_number_value(TreeNode *value, int default_value = 0);

    ObjNode *get_object(int id, int generation);
    ObjNode *get_object(RefNode *ref);
    TreeNode *get_real_value(TreeNode *value);
    TreeNode *get_real_obj_value(TreeNode *value);
    TreeNode *get_named_value(string name);
    void get_stream(ArrayNode *array, stringstream *stream_value);
    void get_stream(ObjNode *obj, stringstream *stream_value);
};
