#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <locale>

using namespace std;

static const wchar_t *_program_name = NULL;
static bool _verbose = false;

void verbose_message(const wchar_t *msg)
{
    if (verbose_mode()) {
        wcout << _program_name << ": " << msg << endl;
    }
}

void set_program_name(const wchar_t *name)
{
    _program_name = name;
}

wchar_t *to_unicode(const char *str)
{
    wchar_t *string = new wchar_t[strlen(str) + 1];
    mbstowcs(string, str, strlen(str));
    return string;
}

void error_message(const wchar_t *msg)
{
    wcerr << _program_name << L": " << msg << endl;
}

void set_verbose_mode(const bool verbose)
{
    _verbose = verbose;
}

bool verbose_mode()
{
    return _verbose;
}
