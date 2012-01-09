#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <locale>

using namespace std;

static wchar_t *program_name = NULL;

void set_program_name(const char *name)
{
    program_name = new wchar_t[strlen(name) + 1];
    mbstowcs(program_name, name, strlen(name));
}

void error_message(const wchar_t *msg)
{
    wcerr << program_name << L": " << msg << endl;
}
