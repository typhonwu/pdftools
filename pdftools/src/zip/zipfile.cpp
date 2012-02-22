#include "config.h"
#include "zipfile.h"
#include "utils.h"
#include <iostream>
#include <cstring>
#include <zlib.h>
#include <ctime>

using namespace std;

uint32_t ZipFile::current_datetime()
{
    time_t rawtime;
    struct tm * t;

    time(&rawtime);
    t = localtime(&rawtime);

    if (t->tm_year >= 1980)
        t->tm_year -= 1980;
    else if (t->tm_year >= 80)
        t->tm_year -= 80;
    return (uint32_t) ((t->tm_mday + (32 * (t->tm_mon + 1)) + (512 * t->tm_year)) << 16) |
            ((t->tm_sec / 2) + (32 * t->tm_min) + (2048 * t->tm_hour));
}

ZipFile::ZipFile()
{
    m_cd_address = 0;
    m_cd_size = 0;
}

ZipFile::~ZipFile()
{
    close();
}

bool ZipFile::open(const char *output)
{
    m_output.open(output, ios::binary);
    return m_output.is_open();
}

void ZipFile::close()
{
    if (m_output.is_open()) {
        write_central_file();
        write_central_directory();
        m_output.flush();
        m_output.close();
    }
}

void ZipFile::add_source(const char *filename, const char *buffer, int length)
{
    if (length == 0) {
        length = strlen(buffer);
    }

    appended_files file;
    file.position = m_output.tellp();
    file.date = current_datetime();
    file.length = length;
    file.name = filename;

    uLong crc = ::crc32(0L, Z_NULL, 0);
    file.crc = ::crc32(crc, (Bytef *) buffer, length);

    // When using the Compress method, ZLib adds a 2 byte head
    // and a 4 byte tail. The head must be removed for zip
    // compatability and the tail is not necessary.
    char *b = deflate(buffer, length, file.compressed_size);
    file.compressed_size -= 6;

    if (file.compressed_size < file.length) {
        file.compressed = true;
    } else {
        file.compressed = false;
        file.compressed_size = file.length;
    }

    write_string("\x50\x4B\x03\x04");
    // Unix Type
    write16(0xA);
    if (file.compressed) {
        // Bit flags
        write16(2);
        // Compression mode
        write16(8);
    } else {
        write16(0);
        write16(0);
    }
    write32(file.date);
    write32(file.crc);
    // Compressed
    write32(file.compressed_size);
    // Uncompressed
    write32(file.length);
    write16(strlen(filename));
    // file extra
    write16(0);
    write_string(filename);

    if (file.compressed) {
        m_output.write(b + 2, file.compressed_size);
    } else {
        m_output.write(buffer, file.length);
    }
    delete [] b;
    m_files.push_back(file);
}

void ZipFile::write_central_file()
{
    int size = m_files.size();
    m_cd_address = m_output.tellp();

    for (int i = 0; i < size; i++) {
        appended_files file = m_files[i];

        write_string("\x50\x4B\x01\x02");
        write16(0x031E);
        write16(0x0A);

        if (file.compressed) {
            // bit flag
            write16(2);
            // compression
            write16(8);
        } else {
            write16(0);
            write16(0);
        }
        write32(file.date);
        write32(file.crc);
        // Compressed
        write32(file.compressed_size);
        // Uncompressed
        write32(file.length);
        write16(file.name.length());
        // file extra
        write16(0);
        // file comment length
        write16(0);
        // dist start
        write16(0);
        write16(0);
        write32(0x81A40000);
        write32(file.position);
        write_string(file.name.c_str());
    }
    m_cd_size = ((uint32_t) m_output.tellp()) - m_cd_address;
}

void ZipFile::write_central_directory()
{
    write_string("\x50\x4B\x05\x06");
    // number of this disk
    write16(0);
    // number of this disk start
    write16(0);
    // number of files records (this disk)
    write16(m_files.size());

    // Total number of central directory records
    write16(m_files.size());
    // size of the central directory
    write32(m_cd_size);

    // offset of start of central
    write32(m_cd_address);

    // Comment size
    write16(0);
}

void ZipFile::write_string(const char *str)
{
    m_output << str;
}

void ZipFile::write8(uint8_t c)
{
    m_output.put(c);
}

void ZipFile::write16(uint16_t c)
{
    m_output.put(c & 0xFF);
    m_output.put((c >> 8) & 0xFF);
}

void ZipFile::write32(uint32_t c)
{
    m_output.put(c & 0xFF);
    m_output.put((c >> 8) & 0xFF);
    m_output.put((c >> 16) & 0xFF);
    m_output.put((c >> 24) & 0xFF);
}
