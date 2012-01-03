#include "parser.h"
#include "scanner.h"
#include <iostream>
#include <cstdlib>

using namespace std;

const wchar_t *_pdf_versions[] = {
    L"PDF-1.1",
    L"PDF-1.2",
    L"PDF-1.3",
    L"PDF-1.4",
    L"PDF-1.5",
    L"PDF-1.6",
    L"PDF-1.7"
};

Parser::Parser() : m_token(NULL), m_valid(false)
{
    m_scanner = new Scanner();
}

Parser::~Parser()
{
    delete m_scanner;
    if (m_token) {
        delete m_token;
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
    if (m_token) {
        delete m_token;
    }
    m_token = m_scanner->next_token();
}

bool Parser::match(TokenType type)
{
    if (m_token && m_token->type() == type) {
        next_token();
    } else {
        wcerr << L"unexpected token: " << m_token->value() << endl;
        return false;
    }
    return true;
}

void Parser::parse()
{
    match(PERCENT);
    if (verify_version()) {
        wcout << L"File version: " << m_version << endl;
         
        while(m_scanner->good()) {
            next_token();
            if (m_token == NULL) {
                break;
            }
            switch(m_token->type()) {
            case PERCENT:
                m_scanner->ignore_line();
                break;
            default:
                wcout << L"Token: " << m_token->value() << endl;                
            }
        }
    } else {
        wcerr << L"File not supported." << endl;
    }
}

bool Parser::is_valid()
{
    return m_valid;
}

bool Parser::verify_version()
{
    int loop;
    wstring line = m_token->value();
    for (loop = 0; loop < 7; loop++) {
        if (line == _pdf_versions[loop]) {
            m_version = _pdf_versions[loop];
            return true;
        }
    }
    return false;
}

long Parser::find_xref()
{
    m_scanner->find_last_xref();
    
    return 0;
}
