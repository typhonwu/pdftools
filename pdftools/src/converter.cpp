#include "converter.h"
#include "parser.h"
#include "utils.h"
#include "analyze.h"
#include <iostream>

using namespace std;

Converter::Converter(const char *filein, const char *format)
{
    m_filein = filein;
    m_fileout = string(filein) + "." + format;
    m_format = format;
    m_syntax_tree = NULL;
    m_document = NULL;


    m_fileout = filein;
    size_t last_dot = m_fileout.find_last_of('.');
    if (last_dot != string::npos) {
        m_fileout = m_fileout.substr(0, last_dot);
    }
    m_fileout += ".";
    m_fileout += format;
}

Converter::~Converter()
{
    if (m_document) delete m_document;
    if (m_syntax_tree) delete m_syntax_tree;
}

void Converter::convert()
{
    Parser parser;
    Analyze analyze;

    if (!parser.open_file(m_filein)) {
        error_message("file not found");
    } else {
        m_syntax_tree = parser.parse();
        m_document = analyze.analyze_tree(m_syntax_tree);
        if (m_document) {
//            cout << "Title: " << m_document->title() << endl;
//            cout << "Subject: " << m_document->subject() << endl;
//            cout << "Author: " << m_document->author() << endl;
        } else {
            error_message("Invalid file");
        }
    }
}
