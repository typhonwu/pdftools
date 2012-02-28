#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ostream>
#include <stdint.h>

using namespace std;

void error_message(string msg);
void verbose_message(string msg);
void set_verbose_mode(const bool verbose);
bool verbose_mode();
char *flat_decode(char *compressed, int size, int &deflated);
char *deflate(const char *raw, int size, int &writed);
string utf16be_to_utf8(string &str);
string charset_to_utf8(string &str);

#endif
