#include "utils.h"
#include "config.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <locale>

using namespace std;

static bool _verbose = false;

void verbose_message(const wchar_t *msg)
{
    if (verbose_mode()) {
        wcout << PACKAGE_NAME << L": " << msg << endl;
    }
}

wchar_t *to_unicode(const char *str)
{
    wchar_t *string = new wchar_t[strlen(str) + 1];
    mbstowcs(string, str, strlen(str));
    string[strlen(str)] = 0;
    return string;
}

void error_message(const wchar_t *msg)
{
    wcerr << PACKAGE_NAME << L": " << msg << endl;
}

void set_verbose_mode(const bool verbose)
{
    _verbose = verbose;
}

bool verbose_mode()
{
    return _verbose;
}
