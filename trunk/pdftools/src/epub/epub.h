#ifndef EPUB_H
#define EPUB_H

#include "generator.h"
#include "zip/zipfile.h"

class EPUB : public Generator {
private:
    ZipFile *m_zipfile;
    
public:
    EPUB();
    virtual ~EPUB();
    virtual bool generate(Document *document, const char *output);

private:
    void generate_container();
    void generate_content();
    void generate_mimetype();
};

#endif