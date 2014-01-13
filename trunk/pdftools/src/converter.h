#pragma once

#include <string>

using namespace std;

class Document;
class RootNode;

class Converter {
private:
    const string m_filein;
    const string m_format;
    string m_fileout;
    RootNode *m_syntax_tree;
    Document *m_document;
    
public:
    Converter(const string& filein, const string& format);
    ~Converter();
    
    void convert();
};
