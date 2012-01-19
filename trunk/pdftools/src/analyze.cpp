#include "analyze.h"
#include "nodes/nodes.h"
#include <iostream>

using namespace std;

Analyze::Analyze()
{
    m_document = NULL;
    m_tree = NULL;
    m_pages = NULL;
}

Analyze::~Analyze()
{
}

void Analyze::analyze_xref()
{
    vector<TreeNode *> root = m_tree->child();
    vector<TreeNode *>::iterator i = root.begin();
    // FIXME look for binary xref (stream)

    // Parse XREF Nodes
    while (i < root.end()) {
        XREFNode *xref = dynamic_cast<XREFNode *> (*i);
        if (xref) {
            MapNode *trailer = dynamic_cast<MapNode *> (xref->trailer());

            m_document->set_root(get_real_value(trailer->get("/Root")));
            m_document->set_info(get_real_value(trailer->get("/Info")));

            map<string, TreeNode *> values = trailer->values();
            map<string, TreeNode *>::iterator m = values.begin();

            ArrayNode *array = dynamic_cast<ArrayNode *> (trailer->get("/ID"));
            if (array) {
                vector<TreeNode *> v = array->values();
                if (v.size() == 2) {
                    m_document->set_id(((StringNode*) v[0])->value(), ((StringNode*) v[1])->value());
                }
            }
        }
        i++;
    }
}

void Analyze::analyze_info()
{
    ObjNode *obj = dynamic_cast<ObjNode *> (m_document->info_node());
    if (obj) {
        MapNode *info = dynamic_cast<MapNode *> (obj->value());
        if (info) {
            m_document->set_title(get_string_value(info->get("/Title")));
            m_document->set_author(get_string_value(info->get("/Author")));
            m_document->set_subject(get_string_value(info->get("/Subject")));
        }
    }
}

void Analyze::analyse_root()
{
    ObjNode *obj_root = dynamic_cast<ObjNode *> (m_document->root_node());
    if (!obj_root) {
        // Invalid file
        return;
    }
    MapNode *catalog = dynamic_cast<MapNode *> (obj_root->value());

    NameNode *name = dynamic_cast<NameNode *> (catalog->get("/Type"));
    if (!name || name->name() != "/Catalog") {
        // Invalid file
        return;
    }
    m_pages = get_real_value(catalog->get("/Pages"));
    m_document->set_lang(get_string_value(catalog->get("/Lang")));

    //FIXME PageLabels, Outlines & StructTreeRoot
}

Document *Analyze::analyze_tree(RootNode * tree)
{
    if (!tree) {
        // Invalid tree
        return NULL;
    }
    m_tree = tree;
    m_document = new Document;

    analyze_xref();
    analyze_info();
    analyse_root();
    analyse_pages();

    return m_document;
}

void Analyze::analyse_pages()
{
    ObjNode *obj_pages = dynamic_cast<ObjNode *> (m_pages);
    if (!obj_pages) {
        // Invalid file
        return;
    }
    MapNode *catalog = dynamic_cast<MapNode *> (obj_pages->value());
    map<string, TreeNode *> values = catalog->values();
    map<string, TreeNode *>::iterator m = values.begin();

    while (m != values.end()) {
        cout << (*m).first << endl;
        m++;
    }
}

TreeNode *Analyze::get_real_value(TreeNode * value)
{
    RefNode *ref = dynamic_cast<RefNode *> (value);
    if (ref) {
        return get_object(ref);
    }
    return value;
}

string Analyze::get_string_value(TreeNode * value)
{
    StringNode *str = dynamic_cast<StringNode *> (value);
    if (str) {
        return str->value();
    }
    return string();
}

ObjNode * Analyze::get_object(RefNode * ref)
{
    if (!ref) {
        return NULL;
    }
    return get_object(ref->id(), ref->generation());
}

ObjNode * Analyze::get_object(int id, int generation)
{
    vector<TreeNode *> root = m_tree->child();
    vector<TreeNode *>::iterator i = root.begin();
    while (i < root.end()) {
        ObjNode *obj = dynamic_cast<ObjNode *> (*i);
        if (obj && obj->this_object(id, generation)) {
            return obj;
        }
        i++;
    }
    // Not found
    return NULL;
}
