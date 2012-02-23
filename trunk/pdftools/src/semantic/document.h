#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "nodes/nodes.h"
#include "font.h"
#include "page.h"
#include "pagelabel.h"
#include "outline.h"
#include <string>
#include <vector>

using namespace std;

class Document {
private:
    bool m_encrypted;
    string m_id;
    string m_title;
    string m_subject;
    string m_author;
    string m_lang;
    TreeNode *m_root;
    TreeNode *m_info;
    Outline *m_outlines;
    vector<Font *> m_fonts;
    vector<Page *> m_pages;
    vector<PageLabel *> m_page_label;
    
public:
    Document();
    ~Document();
    
    void set_id(string first, string second);
    void set_root(TreeNode *root);
    void set_info(TreeNode *info);
    void set_encrypted(bool encrypt);
    
    void set_title(string title);
    void set_subject(string subject);
    void set_author(string author);
    void set_lang(string lang);
    
    void set_outline(Outline *outline);
    Outline *outline();
    
    void add_font(Font *font);
    Font *get_font(const char *name);
    Font *get_font_by_alias(const char *alias);
    
    void add_page(Page *page);
    void add_page_label(PageLabel *label);
    Page *page(int index);
    int pages();
    Page *page(int id, int generation);
    
    bool encrypted();
    string lang();
    string id();
    string title();
    string subject();
    string author();
    TreeNode *root_node();
    TreeNode *info_node();
};

#endif
