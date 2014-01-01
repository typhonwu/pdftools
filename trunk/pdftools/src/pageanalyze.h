#pragma once

#include "graphics/graphicstate.h"
#include "glyphs/glyphs.h"
#include "nodes/nodes.h"

class Font;
class Document;

class PageAnalyze {
private:
    GraphicState m_state;
    Document *m_document;
    Glyph *m_root;
    Font *m_font;

public:
    PageAnalyze(Document *document);
    ~PageAnalyze();

    Glyph *analyze_tree(RootNode *tree);

private:
    void analyze_tree(RootNode *tree, Glyph *parent);
    void analyze_text(TextNode *text, Glyph *parent);
    FontSizeGlyph *analyze_text_matrix(TextMatrixNode *text_matrix);
    FontGlyph *analyze_font(FontNode *font);
};
