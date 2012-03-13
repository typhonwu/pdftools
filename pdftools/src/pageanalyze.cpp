#include "pageanalyze.h"
#include "graphics/graphicstate.h"
#include "glyphs/glyphs.h"
#include "semantic/font.h"
#include "nodes/nodes.h"
#include "semantic/document.h"
#include <cstdlib>

PageAnalyze::PageAnalyze(Document *document)
{
    m_document = document;
    m_font = NULL;
    m_state = new GraphicState;
}

PageAnalyze::~PageAnalyze()
{
    if (m_state)
        delete m_state;
}

Glyph *PageAnalyze::analyze_tree(RootNode *tree)
{
    m_root = new Glyph;
    analyze_tree(tree, m_root);
    return m_root;
}

void PageAnalyze::analyze_tree(RootNode *tree, Glyph *parent)
{
    Glyph *node_parent = parent;
    int size = tree->size();
    for (int loop = 0; loop < size; loop++) {
        TreeNode *node = tree->get(loop);

        if (m_document->tree_root()) {
            BDCNode *bdc = dynamic_cast<BDCNode *>(node);
            if (bdc) {
                if (bdc->name() == "/P") {
                    ParagraphGlyph *p = new ParagraphGlyph;
                    analyze_tree(bdc, p);
                    node_parent->add_child(p);
                } else if (bdc->name() == "/Artifact") {
                    MapNode *attr = dynamic_cast<MapNode *>(bdc->value());
                    NameNode *type = dynamic_cast<NameNode *>(attr->get("/Type"));
                    if (type && type->name() == "/Pagination") {
                        // Ignore
                        continue;
                    } else {
                        analyze_tree(bdc, node_parent);
                    }
                } else {
                    analyze_tree(bdc, node_parent);
                }
                continue;
            }
        } else {
            BDCNode *bdc = dynamic_cast<BDCNode *>(node);
            if (bdc) {
                analyze_tree(bdc, node_parent);
            }
        }
        FontNode *font = dynamic_cast<FontNode *>(node);
        if (font) {
            FontGlyph *font_parent = dynamic_cast<FontGlyph *>(node_parent);
            if (font_parent) {
                node_parent = font_parent->parent();
            }
            FontGlyph *font_glyph = analyze_font(font);
            font_glyph->set_parent(node_parent);
            node_parent->add_child(font_glyph);
            node_parent = font_glyph;
            continue;
        }
        TextNode *text = dynamic_cast<TextNode *>(node);
        if (text) {
            analyze_text(text, node_parent);
            continue;
        }
        StateNode *state = dynamic_cast<StateNode *>(node);
        if (state) {
            if (state->save()) {
                m_state->push();
            } else {
                m_state->pop();
            }
        }
    }
}

FontGlyph *PageAnalyze::analyze_font(FontNode *font)
{
    return new FontGlyph(font->name(), font->size());
}

void PageAnalyze::analyze_text(TextNode *text, Glyph *parent)
{
    parent->add_child(new TextGlyph(text->text()));
}
