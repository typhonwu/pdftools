#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <locale>

using namespace std;

static wchar_t *_program_name = NULL;
static bool _verbose = false;

void set_program_name(const char *name)
{
    _program_name = new wchar_t[strlen(name) + 1];
    mbstowcs(_program_name, name, strlen(name));
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
