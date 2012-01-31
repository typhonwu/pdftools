#ifndef ZIPFILE_H
#define ZIPFILE_H

#include <fstream>

using namespace std;

class ZipFile {
private:
    ofstream m_output;
    
public:
    ZipFile();
    ~ZipFile();
    
    bool open(const char *output);
    bool close();
    void add_source(const char *filename, const char *buffer, int length = 0);
};

#endif
