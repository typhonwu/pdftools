#include "parser.h"
#include "scanner.h"
#include <iostream>
#include <cstdlib>

using namespace std;

const char *_pdf_versions[] = {
    "PDF-1.1",
    "PDF-1.2",
    "PDF-1.3",
    "PDF-1.4",
    "PDF-1.5",
    "PDF-1.6",
    "PDF-1.7"
};

Parser::Parser() : token(NULL), m_valid(false)
{
    //string line;
    //while (m_filein.good()) {
    //	getline(m_filein, line);
    //	cout << line << endl;
    //}
    //verify_version();
    //long xref = find_xref();
    //if (!xref) {
    //	throw runtime_error("XRef not found.");
    //}
    //cout << "XRef: " << xref << endl;
    m_scanner = new Scanner();
}

Parser::~Parser()
{
    delete m_scanner;
    if (token) {
        delete token;
    }
}

bool Parser::open_file(const char *path)
{
    bool ret = m_scanner->open_file(path);
    if (ret) {
        next_token();
    }
    return ret;
}

void Parser::next_token()
{
    if (token) {
        delete token;
    }
    token = m_scanner->next_token();
}

bool Parser::match(TokenType type)
{
    if (token && token->type() == type) {
        next_token();
    } else {
        cerr << "unexpected token: " << token->value() << endl;
        return false;
    }
    return true;
}

void Parser::parse()
{
    match(PERCENT);
    if (verify_version()) {
        cout << "File version: " << version << endl;
        next_token();
        
        find_xref();
        // TODO parse the file
    } else {
        cerr << "File not supported." << endl;
    }
}

bool Parser::is_valid()
{
    return m_valid;
}

bool Parser::verify_version()
{
    int loop;
    string line = token->value();
    for (loop = 0; loop < 7; loop++) {
        if (line == _pdf_versions[loop]) {
            version = _pdf_versions[loop];
            return true;
        }
    }
    return false;
}

long Parser::find_xref()
{
    /*string line;
    long xref = 0;
    while (m_filein.good()) {
        getline(m_filein, line);
        if (line == "startxref") {
            getline(m_filein, line);
            stringstream(line) >> xref;
        }
        //cout << line << endl;
    }
    return xref;*/
    return 0;
}
