#ifndef ZIPFILE_H
#define ZIPFILE_H

#include <fstream>
#include <vector>
#include <stdint.h>

using namespace std;

struct appended_files {
    const char *name;
    int length;
    uint32_t date;
    uint32_t crc;
    uint32_t position;
};

class ZipFile {
private:
    ofstream m_output;
    vector<appended_files> m_files;
    
public:
    ZipFile();
    ~ZipFile();
    
    bool open(const char *output);
    void close();
    void add_source(const char *filename, const char *buffer, int length = 0);
    
private:
    void write_central_file();
    void write_central_directory();
    
    void write8(uint8_t c);
    void write16(uint16_t c);
    void write32(uint32_t c);
    void write_string(const char *str);
    
    uint32_t current_datetime();
    uint32_t crc32(uint8_t byte, uint32_t crc32);
    uint32_t crc32(uint8_t *data, uint32_t size);
};

#endif
