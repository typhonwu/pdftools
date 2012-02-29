#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>

using namespace std;

class Document;
class Font;
class Page;

class Context {
private:
	Document *m_document;
	Page *m_page;
	Font *m_font;

public:
	Context(Document *document);

	void set_current_font(string &alias, int size);
	void set_current_page(Page *page);

	Font *font();
	Document *document();
};

#endif
