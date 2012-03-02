#include "converter.h"
#include "parser.h"
#include "utils.h"
#include "analyze.h"
#include "generator.h"
#include "scanner.h"
#include "semantic/document.h"
#include "nodes/nodes.h"
#include <iostream>
#include <sstream>
#include <fstream>

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
    Parser parser(m_filein);
    Analyze analyze(m_filein);

    if (!parser.is_valid()) {
        error_message(string(m_filein).append(" not found."));
    } else {
        m_syntax_tree = parser.parse();
        m_document = analyze.analyze_tree(m_syntax_tree);
        if (m_document) {
            stringstream msg;
            msg << "Parsing file " << m_filein << " ";
            msg << "Pages: " << m_document->pages();
            msg << " - " << "Title: ";
            if (m_document->title().empty()) {
                msg << "no title";
            } else {
                msg << m_document->title();
            }
            verbose_message(msg.str());

            // Generate output file
            Generator *instance = Generator::get_instance(m_format);
            if (instance) {
                if (!instance->generate(m_document, m_fileout.c_str())) {
                    error_message("Cannot generate output file");
                }
                delete instance;
            }
        } else {
            error_message("Invalid input file");
        }
    }
}
