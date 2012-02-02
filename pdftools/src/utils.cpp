#include "utils.h"
#include "config.h"
#include <dirent.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <zlib.h>
#include <iconv.h>

using namespace std;

static bool _verbose = false;

#define MAX_BUFFER_SIZE 16384

struct buffer_struct {
    char buffer[MAX_BUFFER_SIZE];
    int size;
};

void verbose_message(string msg)
{
    if (verbose_mode()) {
        cout << PACKAGE_NAME << ": " << msg << endl;
    }
}

void error_message(string msg)
{
    cerr << PACKAGE_NAME << ": " << msg << endl;
}

void set_verbose_mode(const bool verbose)
{
    _verbose = verbose;
}

bool verbose_mode()
{
    return _verbose;
}

char *deflate(const char *raw, int size, int &writed)
{
    z_stream zstream;
    vector<buffer_struct> values;
    writed = 0;

    /* allocate deflate state */
    zstream.zalloc = Z_NULL;
    zstream.zfree = Z_NULL;
    zstream.opaque = Z_NULL;
    int err = deflateInit(&zstream, Z_BEST_SPEED);
    if (err != Z_OK) {
        return NULL;
    }

    zstream.avail_in = size;
    zstream.next_in = (Bytef *)raw;

    register int total = 0;
    do {
        buffer_struct b;
        zstream.avail_out = MAX_BUFFER_SIZE;
        zstream.next_out = (Bytef *) b.buffer;

        err = deflate(&zstream, Z_FINISH); /* no bad return value */
        b.size = MAX_BUFFER_SIZE - zstream.avail_out;

        total += b.size;
        values.push_back(b);
    } while (zstream.avail_out == 0);
    //} while (flush != Z_FINISH);
    //assert(ret == Z_STREAM_END);        /* stream will be complete */

    deflateEnd(&zstream);

    writed = total;
    char *ret = new char[total];

    int locate = 0;
    vector<buffer_struct>::iterator i;
    for (i = values.begin(); i != values.end(); i++) {
        memcpy(ret + locate, (*i).buffer, (*i).size);
        locate += (*i).size;
    }
    return ret;
}

char *flat_decode(int8_t *compressed, int size)
{
    vector<buffer_struct> values;

    z_stream zstream;
    zstream.zalloc = Z_NULL;
    zstream.zfree = Z_NULL;
    zstream.opaque = Z_NULL;
    zstream.avail_in = 0;
    zstream.next_in = Z_NULL;

    int total = 0;
    int rsti = inflateInit(&zstream);
    if (rsti == Z_OK) {
        zstream.avail_in = size;
        zstream.next_in = (Bytef *) compressed;

        do {
            buffer_struct b;
            zstream.avail_out = MAX_BUFFER_SIZE;
            zstream.next_out = (Bytef *) b.buffer;

            int rst2 = inflate(&zstream, Z_NO_FLUSH);
            if (rst2 >= 0) {
                b.size = MAX_BUFFER_SIZE - zstream.avail_out;\
                total += b.size;
                values.push_back(b);
                if (rst2 == Z_STREAM_END) break;
            } else {
                cout << "error in decompression " << rst2 << endl;
                // Error in decompression
                break;
            }
        } while (zstream.avail_out == 0);
    }
    inflateEnd(&zstream);

    char *ret = new char[total + 1];
    ret[total] = 0;

    int locate = 0;
    vector<buffer_struct>::iterator i;
    for (i = values.begin(); i != values.end(); i++) {
        memcpy(ret + locate, (*i).buffer, (*i).size);
        locate += (*i).size;
    }
    return ret;
}

string utf16_to_utf8(string &str)
{
    string ret = str;
    bool convert_string = false;
    if (str.length() > 2) {
        uint8_t first = str[0];
        uint8_t second = str[1];
        if ((first == 0xFE && second == 0xFF)
                || (first == 0xFF && second == 0xFE)) {
            // UTF-16LE or UTF-16BE
            convert_string = true;
        }
    }

    if (convert_string) {
        iconv_t conv_desc = iconv_open("UTF-8", "UTF-16");
        if ((size_t) conv_desc == (size_t) - 1) {
            /* Initialization failure. Do not convert strings */
        } else {
            size_t len = str.length();
            size_t utf8len = len * 2;
            char *utf16 = (char*) str.c_str();
            char *utf8 = new char[utf8len];
            char *utf8start = utf8;
            memset(utf8, 0, len);

            size_t iconv_value = iconv(conv_desc, &utf16, &len, & utf8, & utf8len);
            /* Handle failures. */
            if ((int) iconv_value != -1) {
                ret = utf8start;
            }
            delete [] utf8start;
            iconv_close(conv_desc);
        }
    }
    return ret;
}
