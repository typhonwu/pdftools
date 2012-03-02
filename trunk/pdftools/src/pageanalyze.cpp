#include "pageanalyze.h"
#include "glyphs/glyphs.h"
#include "semantic/font.h"
#include "nodes/nodes.h"
#include <cstdlib>

PageAnalyze::PageAnalyze(Document *document) {
    m_document = document;
    m_font = NULL;
}

PageAnalyze::~PageAnalyze() {
}

Glyph *PageAnalyze::analyze_tree(RootNode *tree) {
    m_root = new ParagraphGlyph;

    int size = tree->size();
    for (int loop = 0; loop < size; loop++) {
        TreeNode *node = tree->get(loop);

        FontNode *font = dynamic_cast<FontNode *>(node);
        if (font) {
            //m_root->add_child(new BreakGlyph);
            m_root->add_child(analyze_font(font));
        } else {
            TextNode *text = dynamic_cast<TextNode *>(node);
            if (text) {
                analyze_text(text, m_root);
            }
        }
    }
    return m_root;
}

FontGlyph *PageAnalyze::analyze_font(FontNode *font) {
    return new FontGlyph(font->name(), font->size());
}

void PageAnalyze::analyze_text(TextNode *text, Glyph *parent) {
    string str = text->text();
    parent->add_child(new TextGlyph(str));
}
