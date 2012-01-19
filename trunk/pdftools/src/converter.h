#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include "nodes/nodes.h"
#include "semantic/document.h"

class Converter {
private:
    const char *m_filein;
    const char *m_format;
    std::string m_fileout;
    RootNode *m_syntax_tree;
    Document *m_document;
    
public:
    Converter(const char *filein, const char *format);
    ~Converter();
    
    void convert();
};

#endif
