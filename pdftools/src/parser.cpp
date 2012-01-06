#include "parser.h"
#include "scanner.h"
#include "utils.h"
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
        wstring msg = L"unexpected token: ";
        msg += m_token->value();
        error_message(msg.c_str());
        return false;
    }
    return true;
}

void Parser::parse()
{
    bool error = false;
    match(PERCENT);
    if (verify_version()) {
        while (m_scanner->good() && !error) {
            next_token();
            if (m_token == NULL) {
                break;
            }
            switch (m_token->type()) {
            case PERCENT:
                m_scanner->ignore_line();
                break;
            case NUM:
                object_sequence();
                break;
            default:
                //wcout << L"Token: " << m_token->value() << endl;
                error = true;
            }
        }
    } else {
        error_message(L"invalid input file");
    }
}

void Parser::object_sequence()
{
    wstring first = m_token->value();
    match(NUM);
    wstring sec = m_token->value();
    match(NUM);
    if (m_token->value() != L"obj") {
        wstring msg = L"unexpected token in obj parser: ";
        msg += m_token->value();
        error_message(msg.c_str());
    }
    match(NAME);
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

