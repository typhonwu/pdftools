#include "analyze.h"
#include "utils.h"
#include "scanner.h"
#include "pageparser.h"
#include "nodes/nodes.h"
#include "semantic/outline.h"
#include "semantic/pagelabel.h"
#include "semantic/glyphs.h"
#include <iostream>
#include <zlib.h>
#include <cstdlib>
#include <stdexcept>

using namespace std;

Analyze::Analyze(const char *filein)
{
    m_document = NULL;
    m_tree = NULL;
    m_page_tree = NULL;
    m_filein = filein;
    m_scanner = new Scanner;
}

Analyze::~Analyze()
{
    if (m_scanner) {
        delete m_scanner;
    }
}

void Analyze::analyze_xref()
{
    int i;
    int size = m_tree->size();

#pragma omp parallel for
    for (i = 0; i < size; i++) {
        TreeNode *value = m_tree->get(i);
        XREFNode *xref = dynamic_cast<XREFNode *> (value);
        if (xref) {
            MapNode *trailer = dynamic_cast<MapNode *> (xref->trailer());

            TreeNode *rootValue = get_real_value(trailer->get("/Root"));
            TreeNode *encrypt = get_real_value(trailer->get("/Encrypt"));
            TreeNode *info = get_real_value(trailer->get("/Info"));
            if (rootValue) {
                m_document->set_root(rootValue);
            }
            if (info) {
                m_document->set_info(info);
            }
            if (encrypt) {
                m_document->set_encrypted(true);
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

void Analyze::analyze_root()
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

    MapNode *names = dynamic_cast<MapNode *> (get_real_obj_value(catalog->get("/Names")));
    if (names) {
        MapNode *dests = dynamic_cast<MapNode *> (get_real_obj_value(names->get("/Dests")));
        analyze_names(dests);
    }

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
        analyze_outlines(outlines);
    }
    //FIXME StructTreeRoot
}

void Analyze::analyze_names(MapNode *values)
{
    ArrayNode *kids = dynamic_cast<ArrayNode *> (values->get("/Kids"));
    if (kids) {
        int size = kids->size();
#pragma omp parallel for
        for (int i = 0; i < size; i++) {
            MapNode *map_kids = dynamic_cast<MapNode *> (get_real_obj_value(kids->value(i)));
            analyze_names(map_kids);
        }
    } else {
        ArrayNode *names = dynamic_cast<ArrayNode *> (values->get("/Names"));
        if (names) {
            int size = names->size();
#pragma omp parallel for
            for (int i = 0; i < size; i += 2) {
                string name = get_string_value(names->value(i));
                m_names[name] = names->value(i + 1);
            }
        }
    }
}

TreeNode *Analyze::get_named_value(string name)
{
    try {
        return m_names.at(name);
    } catch (out_of_range &) {
        return NULL;
    }
}

void Analyze::analyze_outlines(MapNode *values, Outline * parent)
{
    NameNode *type = dynamic_cast<NameNode *> (values->get("/Type"));
    if (type) {
        if (type->name() != "/Outlines") {
            error_message("Invalid outlines");
            return;
        }
    }
    Outline *outline = new Outline;

    string named_dest = get_string_value(values->get("/Dest"));
    if (!named_dest.empty()) {
        MapNode *map = dynamic_cast<MapNode *> (get_real_obj_value(get_named_value(named_dest)));
        if (map) {
            ArrayNode *dest = dynamic_cast<ArrayNode *> (get_real_obj_value(map->get("/D")));
            if (dest) {
                RefNode *ref = dynamic_cast<RefNode *> (dest->value(0));
                if (ref) {
                    outline->set_destination(ref->id(), ref->generation());
                }
            }
        }
    }
    ArrayNode *destinations = dynamic_cast<ArrayNode *> (values->get("/Dest"));
    if (destinations && destinations->size() > 0) {
        RefNode *ref = dynamic_cast<RefNode *> (destinations->value(0));
        if (ref) {
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
        analyze_outlines(first, outline);
    }

    MapNode *next = dynamic_cast<MapNode *> (get_real_obj_value(values->get("/Next")));
    if (next && parent) {
        analyze_outlines(next, parent);
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
    if (m_document->encrypted()) {
        error_message("Encrypted file is not supported");
        return NULL;
    } else {
        analyze_info();
        analyze_root();
        analyze_pages(m_page_tree);
        return m_document;
    }
}

Glyph *Analyze::analize_page(TreeNode *node)
{
    BDCNode *bdc = dynamic_cast<BDCNode *> (node);
    if (bdc) {
        if (bdc->name() == "/Artifact") {
            MapNode *map = dynamic_cast<MapNode *> (bdc->value());
            if (map) {
                NameNode *name = dynamic_cast<NameNode *> (map->get("/Subtype"));
                if (name && name->name() == "/Footer") {
                    // Ignore footer
                    return NULL;
                }
            }
        }
        if (bdc->name() == "/P") {
            return new Break;
        }
    } else {
        TextNode *text = dynamic_cast<TextNode *> (node);
        if (text) {
            return new Text(text->text());
        } else {
            BreakNode *b = dynamic_cast<BreakNode *> (node);
            if (b) {
                return new Break;
            }
        }
    }
    return NULL;
}

Page *Analyze::process_page(int id, int generation, stringstream *stream_value, ArrayNode * mediabox)
{
    Page *page = new Page;
    page->set_destination(id, generation);

#ifdef DEBUG
    //cout << stream_value->str() << endl;
#endif

    stream_value->seekg(0);
    PageParser parser(stream_value);
    RootNode *root = parser.parse();

    int size = root->size();
    for (int loop = 0; loop < size; loop++) {
        page->add_glyph(analize_page(root->get(loop)));
    }
    return page;
}

void Analyze::get_stream(ArrayNode *array, stringstream *stream_value)
{
    if (array) {
        for (int loop = 0; loop < array->size(); loop++) {
            ObjNode *obj = dynamic_cast<ObjNode *> (get_real_value(array->value(loop)));
            get_stream(obj, stream_value);
        }
    }
}

void Analyze::get_stream(ObjNode *obj, stringstream *stream_value)
{
    MapNode *node = dynamic_cast<MapNode *> (obj->value());
    NameNode *filter = dynamic_cast<NameNode *> (get_real_obj_value(node->get("/Filter")));
    ArrayNode *filter_array = dynamic_cast<ArrayNode *> (get_real_obj_value(node->get("/Filter")));
    int length = get_number_value(get_real_obj_value(node->get("/Length")));

    ifstream filein;
    filein.open(m_filein, ios::binary);
    m_scanner->set_istream(&filein);
    m_scanner->to_pos(obj->stream_pos());

    char *stream = m_scanner->get_stream(length);
    filein.close();

    int total = length;
    if (filter && filter->name() == "/FlateDecode") {
        const char *value = flat_decode(stream, length, total);
        (*stream_value).write(value, total);
        delete [] value;
    } else if (filter_array) {
        int size = filter_array->size();
        if (size > 1) {
#ifdef DEBUG
            for (int loop = 0; loop < size; loop++) {
                filter = dynamic_cast<NameNode *> (get_real_value(filter_array->value(loop)));
                cout << filter->name() << endl;
            }
#endif
            error_message("More than one filter is not supported.");
        } else {
            for (int loop = 0; loop < size; loop++) {
                filter = dynamic_cast<NameNode *> (get_real_value(filter_array->value(loop)));
                if (filter && filter->name() == "/FlateDecode") {
                    const char *value = flat_decode(stream, length, total);
                    (*stream_value).write(value, total);
                    delete [] value;
                } else {
                    error_message(string("Invalid filter ") + filter->name());
                }
            }
        }
    } else if (!filter) {
        (*stream_value).write(stream, total);
    } else {
        error_message(string("Invalid filter ") + filter->name());
    }
    delete [] stream;
}

void Analyze::analyze_pages(TreeNode *page, ArrayNode * mediabox)
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
                int kids_size = kids->size();
                for (int loop = 0; loop < kids_size; loop++) {
                    analyze_pages(get_real_value(kids->value(loop)), media);
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
                    stringstream stream_value;
                    get_stream(contents, &stream_value);
                    m_document->add_page(process_page(obj_pages->id(), obj_pages->generation(), &stream_value, media));
                } else {
                    stringstream stream_value;
                    ArrayNode *array = dynamic_cast<ArrayNode *> (contents->value());
                    get_stream(array, &stream_value);
                    m_document->add_page(process_page(obj_pages->id(), obj_pages->generation(), &stream_value, media));
                }
            }
        }
    }
}

TreeNode * Analyze::get_real_value(TreeNode * value)
{
    RefNode *ref = dynamic_cast<RefNode *> (value);
    if (ref) {
        return get_object(ref);
    }
    return value;
}

TreeNode * Analyze::get_real_obj_value(TreeNode * value)
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

ObjNode * Analyze::get_object(RefNode * ref)
{
    if (!ref) {
        return NULL;
    }
    return get_object(ref->id(), ref->generation());
}

ObjNode * Analyze::get_object(int id, int generation)
{
    int size = m_tree->size();
    int i;
    ObjNode *ret = NULL;
    bool done = false;

#pragma omp parallel for
    for (i = 0; i < size; i++) {
#pragma omp flush(done)
        if (!done) {
            ObjNode *obj = dynamic_cast<ObjNode *> (m_tree->get(i));
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

