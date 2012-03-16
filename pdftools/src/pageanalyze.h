#pragma once
#ifndef PAGEANALYZE_H
#define PAGEANALYZE_H

#include "glyphs/glyphs.h"
#include "nodes/nodes.h"

class Font;
class Document;

class PageAnalyze {
private:
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
    FontGlyph *analyze_font(FontNode *font);
    TextMatrixGlyph *analyze_text_matrix(TextMatrixNode *text_matrix);
};

#endif
