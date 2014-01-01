#pragma once

class Document;

class Generator {
protected:
    Generator();

public:
    virtual ~Generator();
    static Generator *get_instance(const char *format);
    virtual bool generate(Document *document, const char *output) = 0;
};
