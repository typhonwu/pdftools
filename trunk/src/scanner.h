#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>

class Scanner
{
private:
	std::ifstream m_filein;

public:
	Scanner();
	~Scanner();

	bool open_file(const char *path);
	bool is_open();

private:
	void close_file();
};

#endif
