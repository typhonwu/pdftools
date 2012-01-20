#ifndef UTILS_H
#define UTILS_H

#include <string>

using namespace std;

void error_message(string msg);
void verbose_message(string msg);
void set_verbose_mode(const bool verbose);
bool verbose_mode();

#endif
