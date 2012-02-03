#include "analyze.h"
#include "utils.h"
#include "scanner.h"
#include "pageparser.h"
#include "nodes/nodes.h"
#include "semantic/outline.h"
#include "semantic/pagelabel.h"
#include <iostream>
#include <sstream>
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
                if (array->size() == 2) {
                    m_document->set_id(get_string_value(array->value(0)), get_string_value(array->value(1)));
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
                            if (array->size() == 2) {
                                m_document->set_id(get_string_value(array->value(0)), get_string_value(array->value(1)));
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
            int loop;
            int size = array->size();

            for (loop = 0; loop < size; loop += 2) {
                double page = get_number_value(array->value(loop));
                MapNode *attributes = dynamic_cast<MapNode *> (get_real_obj_value(array->value(loop + 1)));
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

    MapNode *outlines = dynamic_cast<MapNode *> (get_real_obj_value(catalog->get("/Outlines")));
    if (outlines) {
        analyse_outlines(outlines);
    }
    //FIXME StructTreeRoot
}

void Analyze::analyse_outlines(MapNode *values, Outline *parent)
{
    NameNode *type = dynamic_cast<NameNode *> (values->get("/Type"));
    if (type) {
        if (type->name() != "/Outlines") {
            error_message("Invalid outlines");
            return;
        }
    }
    Outline *outline = new Outline;

    ArrayNode *destinations = dynamic_cast<ArrayNode *> (values->get("/Dest"));
    if (destinations && destinations->size() > 0) {

        RefNode *ref = dynamic_cast<RefNode *> (destinations->value(0));
        if (ref) {
            // FIXME process references
            outline->set_destination(ref->id(), ref->generation());
        }
    } else {
        MapNode *actions = dynamic_cast<MapNode *> (get_real_obj_value(values->get("/A")));
        if (actions) {
            NameNode *subtype = dynamic_cast<NameNode *> (get_real_obj_value(actions->get("/S")));
            if (subtype && subtype->name() == "/GoTo") {
                ArrayNode *dest = dynamic_cast<ArrayNode *> (get_real_obj_value(actions->get("/D")));
                if (dest) {
                    RefNode *ref = dynamic_cast<RefNode *> (dest->value(0));
                    if (ref) {
                        // FIXME process references
                        outline->set_destination(ref->id(), ref->generation());
                    }
                }
            }
        }
    }
    outline->set_title(get_string_value(values->get("/Title")));

    if (!parent) {
        // root node
        m_document->set_outline(outline);
    } else {
        if (parent) {
            parent->add_child(outline);
        }
    }
    MapNode *first = dynamic_cast<MapNode *> (get_real_obj_value(values->get("/First")));
    if (first) {
        analyse_outlines(first, outline);
    }

    MapNode *next = dynamic_cast<MapNode *> (get_real_obj_value(values->get("/Next")));
    if (next && parent) {
        analyse_outlines(next, parent);
    }

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

// 193

Page *Analyze::process_page(int id, int generation, ObjNode *obj, MapNode *node, ArrayNode *mediabox)
{
    Page *page = new Page;

    const char *uncompressed;
    NameNode *filter = dynamic_cast<NameNode *> (get_real_value(node->get("/Filter")));
    if (filter && filter->name() == "/FlateDecode") {
        uncompressed = flat_decode(obj->stream(), obj->stream_size());
    } else if (!filter) {
        uncompressed = (char *) obj->stream();
    } else {
        error_message(string("Invalid filter ") + filter->name());
        return page;
    }
    stringstream stream_value;
    stream_value << uncompressed;
    stream_value.seekg(0);

    page->set_destination(id, generation);

    // FIXME page parser
    //PageParser parser(stream_value);
    // RootNode *root = parser.parse();

    if (!filter) {
        obj->clear_stream();
    } else {

        delete [] uncompressed;
    }
    return page;
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
            ArrayNode *media = dynamic_cast<ArrayNode *> (catalog->get("/MediaBox"));
            if (!media) {
                media = mediabox;
            }
            if (kids) {
#pragma omp parallel for
                for (int loop = 0; loop < kids->size(); loop++) {
                    analyse_pages(get_real_value(kids->value(loop)), media);
                }
            }
        } else if (type->name() == "/Page") {
            ArrayNode *media = dynamic_cast<ArrayNode *> (catalog->get("/MediaBox"));
            if (!media) {
                media = mediabox;
            }

            ObjNode *contents = dynamic_cast<ObjNode *> (get_real_value(catalog->get("/Contents")));
            if (contents) {
                MapNode *snode = dynamic_cast<MapNode *> (contents->value());
                if (snode) {
                    m_document->add_page(process_page(obj_pages->id(), obj_pages->generation(), contents, snode, media));
                } else {
                    ArrayNode *array = dynamic_cast<ArrayNode *> (contents->value());
                    if (array) {
                        // FIXME more than one content

                        error_message("More than on content is unsupported");
                    }
                }
            }


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

