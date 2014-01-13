#pragma once

#include <string>

class Document;

class Generator {
protected:
    Generator();

public:
    virtual ~Generator();
    static Generator *get_instance(const std::string& format);
    virtual bool generate(Document *document, const std::string& output) = 0;
};
