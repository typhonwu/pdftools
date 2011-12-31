#include "scanner.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif

Scanner::Scanner()
{
}

Scanner::~Scanner()
{
	close_file();
}

void Scanner::close_file()
{
	if (is_open()) {
                m_filein.close();
        }
}

bool Scanner::is_open()
{
	return m_filein.is_open();
}

bool Scanner::open_file(const char *path)
{
	close_file();
	m_filein.open(path);
	return is_open();
}

