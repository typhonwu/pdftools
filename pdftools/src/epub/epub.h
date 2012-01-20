#ifndef EPUB_H
#define EPUB_H

#include "../generator.h"

class EPUB : public Generator {
public:
    EPUB();
    virtual void generate(Document *document, const char *output);
};

#endif
