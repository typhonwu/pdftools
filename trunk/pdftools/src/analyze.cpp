#include "analyze.h"
#include "nodes/nodes.h"
#include "semantic/pagelabel.h"
#include "utils.h"
#include <iostream>
#include <zlib.h>
#include <cstdlib>

using namespace std;

Analyze::Analyze()
{
    m_document = NULL;
    m_tree = NULL;
    m_page_tree = NULL;
}

Analyze::~Analyze()
{
}

void Analyze::analyze_xref()
{
    vector<TreeNode *> root = m_tree->child();
    int i;
    int size = root.size();

#pragma omp parallel for
    for (i = 0; i < size; i++) {
        TreeNode *value = root[i];
        XREFNode *xref = dynamic_cast<XREFNode *> (value);
        if (xref) {
            MapNode *trailer = dynamic_cast<MapNode *> (xref->trailer());

            TreeNode *rootValue = get_real_value(trailer->get("/Root"));
            TreeNode *info = get_real_value(trailer->get("/Info"));
            if (rootValue) {
                m_document->set_root(rootValue);
            }
            if (info) {
                m_document->set_info(info);
            }

            ArrayNode *array = dynamic_cast<ArrayNode *> (trailer->get("/ID"));
            if (array) {
                vector<TreeNode *> v = array->values();
                if (v.size() == 2) {
                    m_document->set_id(((StringNode*) v[0])->value(), ((StringNode*) v[1])->value());
                }
            }
        } else {
            ObjNode *obj = dynamic_cast<ObjNode *> (value);
            if (obj) {
                MapNode *values = dynamic_cast<MapNode *> (obj->value());
                if (values) {
                    NameNode *type = dynamic_cast<NameNode *> (values->get("/Type"));
                    // analyze only XREF Objects
                    if (type && type->name() == "/XRef") {
                        TreeNode *rootValue = get_real_value(values->get("/Root"));
                        TreeNode *info = get_real_value(values->get("/Info"));
                        if (rootValue) {
                            m_document->set_root(rootValue);
                        }
                        if (info) {
                            m_document->set_info(info);
                        }

                        ArrayNode *array = dynamic_cast<ArrayNode *> (values->get("/ID"));
                        if (array) {
                            vector<TreeNode *> v = array->values();
                            if (v.size() == 2) {
                                m_document->set_id(((StringNode*) v[0])->value(), ((StringNode*) v[1])->value());
                            }
                        }
                    }
                }
            }
        }
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
    m_page_tree = get_real_value(catalog->get("/Pages"));
    m_document->set_lang(get_string_value(catalog->get("/Lang")));

    MapNode *page_labels = dynamic_cast<MapNode *> (get_real_obj_value(catalog->get("/PageLabels")));
    if (page_labels) {
        ArrayNode *array = dynamic_cast<ArrayNode *> (get_real_value(page_labels->get("/Nums")));
        if (array) {
            vector<TreeNode *> values = array->values();
            int loop;
            int size = values.size();

            for (loop = 0; loop < size; loop += 2) {
                double page = get_number_value(values[loop]);
                MapNode *attributes = dynamic_cast<MapNode *> (get_real_obj_value(values[loop + 1]));
                if (attributes) {
                    NameNode *name_type = dynamic_cast<NameNode *> (attributes->get("/S"));
                    page_type type = ARABIC;
                    if (name_type) {
                        if (name_type->name() == "D") {
                            type = ARABIC;
                        } else if (name_type->name() == "R") {
                            type = UPCASE_ROMAN;
                        } else if (name_type->name() == "r") {
                            type = LOWCASE_ROMAN;
                        } else if (name_type->name() == "A") {
                            type = UPCASE_LETTERS;
                        } else if (name_type->name() == "A") {
                            type = LOWCASE_LETTERS;
                        }
                    }
                    string name = get_string_value(attributes->get("/P"));
                    int range = get_number_value(attributes->get("/St"), 1);
                    m_document->add_page_label(new PageLabel(page, range, type, name));
                }
            }
        }
    }
    //FIXME Outlines
    //FIXME StructTreeRoot
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
    analyse_pages(m_page_tree);

    return m_document;
}

void Analyze::process_pages()
{

}

void Analyze::analyse_pages(TreeNode *page, ArrayNode *mediabox)
{
    ObjNode *obj_pages = dynamic_cast<ObjNode *> (page);
    if (!obj_pages) {
        // Invalid file
        return;
    }
    MapNode *catalog = dynamic_cast<MapNode *> (obj_pages->value());

    NameNode *type = dynamic_cast<NameNode *> (catalog->get("/Type"));
    if (type) {
        if (type->name() == "/Pages") {
            ArrayNode *kids = dynamic_cast<ArrayNode *> (catalog->get("/Kids"));
            //NumberNode *count = dynamic_cast<NumberNode *> (catalog->get("/Count"));
            ArrayNode *media = dynamic_cast<ArrayNode *> (catalog->get("/MediaBox"));
            if (!media) {
                media = mediabox;
            }
            if (kids) {
                vector<TreeNode *> pages = kids->values();
                vector<TreeNode *>::iterator i = pages.begin();
                while (i != pages.end()) {
                    analyse_pages(get_real_value(*i), media);
                    i++;
                }
            }
        } else if (type->name() == "/Page") {
            Page *page = new Page;

            ArrayNode *media = dynamic_cast<ArrayNode *> (catalog->get("/MediaBox"));
            if (!media) {
                media = mediabox;
            }
            // FIXME correct bounds verification
            if (media) {
                vector<TreeNode *> bounds = media->values();
                page->set_media_box(get_number_value(bounds[0]), get_number_value(bounds[1]), get_number_value(bounds[2]), get_number_value(bounds[3]));
            }
            ObjNode *contents = dynamic_cast<ObjNode *> (get_real_value(catalog->get("/Contents")));
            if (contents) {
                MapNode *snode = dynamic_cast<MapNode *> (contents->value());
                //NumberNode *length = dynamic_cast<NumberNode *> (get_real_value(snode->get("/Length")));
                if (snode) {
                    // FIXME no filter, FlateDecode or Array
                    NameNode *filter = dynamic_cast<NameNode *> (get_real_value(snode->get("/Filter")));
                    if (filter && filter->name() == "/FlateDecode") {
                        contents->set_uncompressed(flat_decode(contents->stream(), contents->stream_size()));
                        contents->clear_stream();
                        //cout << contents->uncompressed() << endl;
                    }
                } else {
                    // FIXME Array of Object Streams
                    //ArrayNode *contents2 = dynamic_cast<ArrayNode *> (contents->value());
                }
            }
            //ArrayNode *media = dynamic_cast<ArrayNode *> (catalog->get("/Resources"));
            // /Metadata // stream
            // /TemplateInstantiated name
            // /UserUnit number

            m_document->add_page(page);
        }
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

TreeNode *Analyze::get_real_obj_value(TreeNode *value)
{
    RefNode *ref = dynamic_cast<RefNode *> (value);
    if (ref) {
        ObjNode *node = get_object(ref);
        if (node) {
            return node->value();
        }
        return NULL;
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

double Analyze::get_number_value(TreeNode *value, int default_value)
{
    NumberNode *num = dynamic_cast<NumberNode *> (value);
    if (num) {
        return num->value();
    }
    return default_value;
}

ObjNode *Analyze::get_object(RefNode * ref)
{
    if (!ref) {
        return NULL;
    }
    return get_object(ref->id(), ref->generation());
}

ObjNode *Analyze::get_object(int id, int generation)
{
    vector<TreeNode *> root = m_tree->child();
    int size = root.size();
    int i;
    ObjNode *ret = NULL;
    bool done = false;

#pragma omp parallel for
    for (i = 0; i < size; i++) {
#pragma omp flush(done)
        if (!done) {
            ObjNode *obj = dynamic_cast<ObjNode *> (root[i]);
            if (obj && obj->this_object(id, generation)) {
                // Value found
                done = true;
#pragma omp flush(done)
                ret = obj;
            }
        }
    }
    return ret;
}

