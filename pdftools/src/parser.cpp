#include "parser.h"
#include "scanner.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

Parser::Parser()
{
	//string line;
	//while (m_filein.good()) {
	//	getline(m_filein, line);
	//	cout << line << endl;
	//}
	//verify_version();
	//long xref = find_xref();
	//if (!xref) {
	//	throw runtime_error("XRef not found.");
	//}
	//cout << "XRef: " << xref << endl;
	m_scanner = new Scanner();
	m_valid = false;
}

Parser::~Parser()
{
	delete m_scanner;
}

bool Parser::open_file(const char *path)
{
	return m_scanner->open_file(path);
}

void Parser::parse()
{
	
}

bool Parser::is_valid()
{
	return m_valid;
}

/*
const char *_versions[] = {
	"%PDF-1.1",
	"%PDF-1.2",
	"%PDF-1.3",
	"%PDF-1.4",
	"%PDF-1.5",
	"%PDF-1.6",
	"%PDF-1.7"
};

void Parser::verify_version()
{
	string line;
	getline(m_filein, line);
	int loop;
	
	for (loop = 0; loop < 7; loop++) {
		if (line == _versions[loop]) {
			cout << "Version: PDF 1." << loop + 1 << endl;
			return;
		}
	}
	throw runtime_error("Invalid file.");
}

long Parser::find_xref()
{
	string line;
	long xref = 0;
	while (m_filein.good()) {
		getline(m_filein, line);
		if (line == "startxref") {
			getline(m_filein, line);
			stringstream(line) >> xref;
		}
		//cout << line << endl;
    }
    return xref;
}
*/

