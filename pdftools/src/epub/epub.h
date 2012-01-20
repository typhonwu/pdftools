#ifndef EPUB_H
#define EPUB_H

#include "../generator.h"
#include <zip.h>

class EPUB : public Generator {
private:
    zip *m_zipfile;
    
public:
    EPUB();
    ~EPUB();
    virtual void generate(Document *document, const char *output);

private:
    void generate_container();
    void generate_mimetype();
};

#endif
