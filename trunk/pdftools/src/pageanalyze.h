#pragma once
#ifndef PAGEANALYZE_H
#define PAGEANALYZE_H

#include "nodes/nodes.h"

class Document;
class Glyph;

class PageAnalyze {
private:
	Document *m_document;
	Glyph *m_root;

public:
	PageAnalyze(Document *document);
	~PageAnalyze();

	Glyph *analyze_tree(RootNode *tree);

private:
	void analyze_font(FontNode *font);
	void analyze_text(TextNode *text);
};

#endif
