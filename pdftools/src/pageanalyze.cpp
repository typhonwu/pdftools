#include "pageanalyze.h"
#include "glyphs/glyphs.h"
#include "semantic/font.h"
#include "nodes/nodes.h"
#include <cstdlib>

PageAnalyze::PageAnalyze(Document *document)
{
	m_document = document;
	m_font = NULL;
}

PageAnalyze::~PageAnalyze()
{
}

Glyph *PageAnalyze::analyze_tree(RootNode *tree)
{
	m_root = new Glyph;
	ParagraphGlyph *p = NULL;

	int size = tree->size();
	for (int loop = 0; loop < size; loop++) {
		TreeNode *node = tree->get(loop);

		if (!p) {
			p = new ParagraphGlyph;
			m_root->add_child(p);
		}

		FontNode *font = dynamic_cast<FontNode *>(node);
		if (font) {
			p->add_child(analyze_font(font));
		} else {
			TextNode *text = dynamic_cast<TextNode *>(node);
			if (text) {
				p->add_child(analyze_text(text));
			}
		}
	}
	return m_root;
}

FontGlyph *PageAnalyze::analyze_font(FontNode *font)
{
	return new FontGlyph(font->name(), font->size());
}

TextGlyph *PageAnalyze::analyze_text(TextNode *text)
{
	return new TextGlyph(text->text());
}
