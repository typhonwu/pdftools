#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <stdint.h>

using namespace std;

void error_message(string msg);
void verbose_message(string msg);
void set_verbose_mode(const bool verbose);
bool verbose_mode();
char *flat_decode(int8_t *compressed, int size);
string utf16_to_utf8(string &str);

#endif
