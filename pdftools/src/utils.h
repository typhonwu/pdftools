#ifndef UTILS_H
#define UTILS_H

void error_message(const wchar_t *msg);
void verbose_message(const wchar_t *msg);
void set_verbose_mode(const bool verbose);
bool verbose_mode();
wchar_t *to_unicode(const char *str);

#endif
