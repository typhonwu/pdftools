#include "analyze.h"
#include "nodes/nodes.h"
#include <iostream>
#include <zlib.h>

#include "utils.h"

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
    vector<TreeNode *>::iterator i = root.begin();

    // Parse XREF Nodes
    while (i < root.end()) {
        XREFNode *xref = dynamic_cast<XREFNode *> (*i);
        if (xref) {
            MapNode *trailer = dynamic_cast<MapNode *> (xref->trailer());

            TreeNode *root = get_real_value(trailer->get("/Root"));
            TreeNode *info = get_real_value(trailer->get("/Info"));
            if (root) {
                m_document->set_root(root);
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
            ObjNode *obj = dynamic_cast<ObjNode *> (*i);
            if (obj) {
                MapNode *values = dynamic_cast<MapNode *> (obj->value());
                if (values) {
                    NameNode *type = dynamic_cast<NameNode *> (values->get("/Type"));
                    // analyze only XREF Objects
                    if (type && type->name() == "/XRef") {
                        TreeNode *root = get_real_value(values->get("/Root"));
                        TreeNode *info = get_real_value(values->get("/Info"));
                        if (root) {
                            m_document->set_root(root);
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
    m_page_tree = get_real_value(catalog->get("/Pages"));
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
            vector<TreeNode *> bounds = media->values();
            page->set_media_box(get_number_value(bounds[0]), get_number_value(bounds[1]), get_number_value(bounds[2]), get_number_value(bounds[3]));

            ObjNode *contents = dynamic_cast<ObjNode *> (get_real_value(catalog->get("/Contents")));
            if (contents) {
                MapNode *snode = dynamic_cast<MapNode *> (contents->value());
                //NumberNode *length = dynamic_cast<NumberNode *> (get_real_value(snode->get("/Length")));
                NameNode *filter = dynamic_cast<NameNode *> (get_real_value(snode->get("/Filter")));
                if (filter && filter->name() == "/FlateDecode") {
                    contents->set_uncompressed(flat_decode(contents->stream(), contents->stream_size()));
                    contents->clear_stream();
                    //cout << contents->uncompressed() << endl;
                } else {
                    cout << "compression not supported: ";
                    if (filter) {
                        cout << filter->name();
                    } else {
                        cout << "uncompressed";
                    }
                    cout << endl;
                    abort();
                }
                /*vector<char *> values;
                
                // 2k buffer, last byte is for \0 (end string)
                char buffer[2049];
                z_stream zstream;
                memset(&zstream, 0, sizeof (z_stream));
                
                if (contents->id() == 161 && contents->generation() == 0) {
                    cout << endl;
                }

                zstream.avail_in = contents->stream_size();
                zstream.avail_out = sizeof(buffer) - 1;
                zstream.next_in = (Bytef *) contents->stream();
                zstream.next_out = (Bytef *) buffer;

                int count = 0;
                int rsti = inflateInit(&zstream);
                if (rsti == Z_OK) {
                    do {
                        count++;
                        zstream.avail_out = sizeof(buffer) - 1;
                        memset(buffer, 0, sizeof(buffer));
                        zstream.next_out = (Bytef *) buffer;
            
                        int rst2 = inflate(&zstream, Z_NO_FLUSH);
                        if (rst2 >= 0) {
                            char *temp = new char[zstream.total_out];
                            memcmp(temp, buffer, zstream.total_out);
                            values.push_back(temp);
                            //cout << buffer;
                            if (rst2 == Z_STREAM_END) break;
                        } else {
                            cout << endl << "Total loops " << count << endl;
                            cout << endl << rst2 << endl << contents->id() << " " << contents->generation()
                                    << endl << contents->stream_size() << ":" << length->value()
                                    << endl;// << (char*) contents->stream() << endl;
                            abort();
                        }
                    } while(zstream.avail_out == 0);
                }
                vector<char *>::iterator i = values.begin();
                while(i != values.end()) {
                    delete [] (*i);
                    i = values.erase(i);
                }
                inflateEnd(&zstream);*/
            } else {
                ArrayNode *contents2 = dynamic_cast<ArrayNode *> (catalog->get("/Contents"));
                cout << "handle array content" << endl;
            }
            //ArrayNode *media = dynamic_cast<ArrayNode *> (catalog->get("/Resources"));
            //ArrayNode *media = dynamic_cast<ArrayNode *> (catalog->get("/Contents")); // stream or array
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

string Analyze::get_string_value(TreeNode * value)
{
    StringNode *str = dynamic_cast<StringNode *> (value);
    if (str) {
        return str->value();
    }
    return string();
}

double Analyze::get_number_value(TreeNode *value)
{
    NumberNode *num = dynamic_cast<NumberNode *> (value);
    if (num) {
        return num->value();
    }
    return 0;
}

ObjNode *Analyze::get_object(RefNode * ref)
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
