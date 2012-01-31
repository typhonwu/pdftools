#ifndef GENERATOR_H
#define GENERATOR_H

#include "semantic/document.h"

class Generator {
protected:
    Generator();

public:
    virtual ~Generator();
    static Generator *get_instance(const char *format);
    virtual void generate(Document *document, const char *output) = 0;
};

#endif
