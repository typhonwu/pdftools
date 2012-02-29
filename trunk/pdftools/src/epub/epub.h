#ifndef EPUB_H
#define EPUB_H

#include "generator.h"
#include "zip/zipfile.h"

class Context;
class Outline;
class Page;
class XML;

class EPUB : public Generator {
private:
    ZipFile *m_zipfile;
    Context *m_context;
    int m_order;
    
public:
    EPUB();
    virtual ~EPUB();
    virtual bool generate(Document *document, const char *output);

private:
    void generate_container();
    void generate_page(Page *page);
    void generate_content(const char* output);
    void generate_toc(const char* output);
    void generate_outline(XML *xml, Outline *outline);
    void generate_mimetype();
};

#endif
