#pragma once
#ifndef FONTGLYPH_H
#define FONTGLYPH_H

#include <string>
#include "glyph.h"

using namespace std;

class FontGlyph : public Glyph {
private:
	int m_size;
	string m_font;

public:
	FontGlyph(string font, int size);

	virtual void do_glyph(Html *document);

	int size();
	string font();
};

#endif
