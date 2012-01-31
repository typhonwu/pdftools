#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "nodes/nodes.h"
#include "page.h"
#include "pagelabel.h"
#include <string>
#include <vector>

using namespace std;

class Document {
private:
    string m_id;
    string m_title;
    string m_subject;
    string m_author;
    string m_lang;
    TreeNode *m_root;
    TreeNode *m_info;
    vector<Page *> m_pages;
    vector<PageLabel *> m_page_label;
    
public:
    Document();
    ~Document();
    
    void set_id(string first, string second);
    void set_root(TreeNode *root);
    void set_info(TreeNode *info);
    
    void set_title(string title);
    void set_subject(string subject);
    void set_author(string author);
    void set_lang(string lang);
    
    void add_page(Page *page);
    void add_page_label(PageLabel *label);
    vector<Page *> pages();
    
    string lang();
    string id();
    string title();
    string subject();
    string author();
    TreeNode *root_node();
    TreeNode *info_node();
};

#endif
