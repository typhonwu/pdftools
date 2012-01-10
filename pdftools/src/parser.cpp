#include "parser.h"
#include "scanner.h"
#include "utils.h"
#include "treenode.h"
#include <iostream>
#include <cstdlib>
#include <string>

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

Parser::Parser() : m_token(NULL), m_valid(false), m_linear(false)
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

bool Parser::match(TokenType type, bool readNext)
{
    if (m_token && m_token->type() == type) {
        if (readNext) {
            next_token();
        }
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
                comment_sequence();
                break;
            case NUM:
                object_sequence();
                break;
            case XREF:
                xref_sequence();
                break;
            default:
                wcout << L"Token: " << m_token->value() << endl;
                error = true;
            }
        }
    } else {
        error_message(L"invalid input file");
    }
}

void Parser::comment_sequence()
{
    m_scanner->ignore_line();
}

void Parser::xref_sequence()
{
    match(XREF);

    while (m_token->type() != TREILER) {
        wstring start = m_token->value();
        match(NUM);
        wstring count = m_token->value();
        match(NUM);
        
        // for ...
    }
}

void Parser::object_sequence()
{
    wstring number = m_token->value();
    match(NUM);
    wstring generation_nunber = m_token->value();
    match(NUM);
    match(OBJ);
    match(START_DICT);

    switch (m_token->type()) {
    case LINEARIZED:
        linear_sequence();
        break;
    }
    match(END_OBJ, false);
}

void Parser::linear_sequence()
{
    match(LINEARIZED);
    wstring version = m_token->value();
    match(NUM);

    while (m_token->type() != END_DICT) {
        if (m_token->value() == L"/L") {
            // File length
            match(NAME);
            match(NUM);
        } else if (m_token->value() == L"/H") {
            match(NAME);
            match(START_ARRAY);
            match(NUM);
            match(NUM);
            match(END_ARRAY);
        } else if (m_token->value() == L"/O") {
            // Fist page object
            match(NAME);
            match(NUM);
        } else if (m_token->value() == L"/E") {
            match(NAME);
            match(NUM);
        } else if (m_token->value() == L"/N") {
            // Pages in document
            match(NAME);
            match(NUM);
        } else if (m_token->value() == L"/T") {
            match(NAME);
            match(NUM);
        } else {
            error_message(L"error parsing the linear mode");
            return;
        }
    }
    match(END_DICT);
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

