#include "pageanalyze.h"
#include "glyphs/glyphs.h"
#include "semantic/font.h"
#include "nodes/nodes.h"
#include <cstdlib>

PageAnalyze::PageAnalyze(Document *document)
{
	m_document = document;
}

PageAnalyze::~PageAnalyze()
{
}

Glyph *PageAnalyze::analyze_tree(RootNode *tree)
{
	m_root = new Glyph;
	int size = tree->size();
	for (int loop = 0; loop < size; loop++) {
		TreeNode *node = tree->get(loop);

		FontNode *font = dynamic_cast<FontNode *>(node);
		if (font) {
			analyze_font(font);
		} else {
			TextNode *text = dynamic_cast<TextNode *>(node);
			if (text) {
				analyze_text(text);
			}
		}
	}
	return m_root;
}

void PageAnalyze::analyze_font(FontNode *font)
{

}

void PageAnalyze::analyze_text(TextNode *text)
{

}
