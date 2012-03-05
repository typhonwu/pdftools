#include "pageanalyze.h"
#include "graphics/graphicstate.h"
#include "glyphs/glyphs.h"
#include "semantic/font.h"
#include "nodes/nodes.h"
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
    m_root = new ParagraphGlyph;

    int size = tree->size();
    for (int loop = 0; loop < size; loop++) {
        TreeNode *node = tree->get(loop);

        FontNode *font = dynamic_cast<FontNode *>(node);
        if (font) {
            m_root->add_child(analyze_font(font));
        }
        TextNode *text = dynamic_cast<TextNode *>(node);
        if (text) {
            analyze_text(text, m_root);
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
    return m_root;
}

FontGlyph *PageAnalyze::analyze_font(FontNode *font)
{
    return new FontGlyph(font->name(), font->size());
}

void PageAnalyze::analyze_text(TextNode *text, Glyph *parent)
{
    parent->add_child(new TextGlyph(text->text()));
}
