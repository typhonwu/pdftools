#include "config.h"
#include "zipfile.h"
#include <iostream>

using namespace std;

ZipFile::ZipFile()
{
}

ZipFile::~ZipFile()
{
    if (m_output.is_open()) {
        m_output.close();
    }
}

bool ZipFile::open(const char *output)
{
    m_output.open(output, ios::binary);
    return m_output.is_open();
}

bool ZipFile::close()
{
    
}

void ZipFile::add_source(const char *filename, const char *buffer, int length)
{
    
}
