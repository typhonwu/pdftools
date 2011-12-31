#ifndef PARSER_H
#define PARSER_H

class Scanner;

class Parser
{
private:
	Scanner *m_scanner;
	bool m_valid;

public:
	Parser();;
	~Parser();

	bool open_file(const char *path);	
	void parse();
	bool is_valid();

private:
	void verify_version();
	long find_xref();
};

#endif
