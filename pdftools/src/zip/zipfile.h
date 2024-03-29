#pragma once
#ifndef ZIPFILE_H
#define ZIPFILE_H

#include <fstream>
#include <vector>
#include <stdint.h>
#include <string>

using namespace std;

struct appended_files {
    string name;
    int length;
    bool compressed;
    int compressed_size;
    uint32_t date;
    uint32_t crc;
    uint32_t position;
};

class ZipFile {
private:
    ofstream m_output;
    vector<appended_files> m_files;
    uint32_t m_cd_address;
    uint32_t m_cd_size;
    
public:
    ZipFile();
    ~ZipFile();
    
    bool open(const string& output);
    void close();
    void add_source(const char *filename, const char *buffer, int length = 0);
    
private:
    void write_central_file();
    void write_central_directory();
    
    void write8(uint8_t c);
    void write16(uint16_t c);
    void write32(uint32_t c);
    void write_string(const string& str);
    
    uint32_t current_datetime() const;
};

#endif
