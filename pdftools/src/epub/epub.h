#ifndef EPUB_H
#define EPUB_H

#include "../generator.h"

class EPUB : public Generator {
private:
    
    
public:
    EPUB();
    ~EPUB();
    virtual void generate(Document *document, const char *output);

private:
    void generate_container();
    void generate_content();
    void generate_mimetype();
};

#endif
