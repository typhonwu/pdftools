#pragma once
#ifndef PAGEANALYZE_H
#define PAGEANALYZE_H

class Document;
class Glyph;
class FontGlyph;
class TextGlyph;
class Font;
class TreeNode;
class FontNode;
class TextNode;
class RootNode;

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
	FontGlyph *analyze_font(FontNode *font);
	void analyze_text(TextNode *text, Glyph *parent);
};

#endif
