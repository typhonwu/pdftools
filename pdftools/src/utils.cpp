#include "utils.h"
#include "config.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

static bool _verbose = false;

void verbose_message(const char *msg)
{
    if (verbose_mode()) {
        cout << PACKAGE_NAME << ": " << msg << endl;
    }
}

void error_message(const char *msg)
{
    cerr << PACKAGE_NAME << ": " << msg << endl;
}

void set_verbose_mode(const bool verbose)
{
    _verbose = verbose;
}

bool verbose_mode()
{
    return _verbose;
}
