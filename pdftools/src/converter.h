#pragma once

#include <string>

using namespace std;

class Document;
class RootNode;

class Converter {
private:
    const char *m_filein;
    const char *m_format;
    string m_fileout;
    RootNode *m_syntax_tree;
    Document *m_document;
    
public:
    Converter(const char *filein, const char *format);
    ~Converter();
    
    void convert();
};
