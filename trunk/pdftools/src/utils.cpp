#include "utils.h"
#include "config.h"
#include <dirent.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <zlib.h>

using namespace std;

static bool _verbose = false;

struct buffer_struct {
    char *buffer;
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

char *flat_decode(int8_t *compressed, int size)
{
    vector<buffer_struct> values;

    // 2k buffer, last byte is for \0 (end string)
    char buffer[2048];
    z_stream zstream;
    memset(&zstream, 0, sizeof (z_stream));

    zstream.avail_in = size;
    zstream.avail_out = sizeof (buffer);
    zstream.next_in = (Bytef *) compressed;
    zstream.next_out = (Bytef *) buffer;

    int total = 0;
    int rsti = inflateInit(&zstream);
    if (rsti == Z_OK) {
        do {
            zstream.avail_out = sizeof (buffer);
            memset(buffer, 0, sizeof (buffer));
            zstream.next_out = (Bytef *) buffer;

            int rst2 = inflate(&zstream, Z_NO_FLUSH);
            if (rst2 >= 0) {
                buffer_struct b;
                b.size = sizeof (buffer) - zstream.avail_out;
                b.buffer = new char[size];
                memcpy(b.buffer, buffer, b.size);
                total += b.size;
                values.push_back(b);
                if (rst2 == Z_STREAM_END) break;
            } else {
                cout << "error" << endl;
                // Error in decompression
                break;
            }
        } while (zstream.avail_out == 0);
    }
    inflateEnd(&zstream);
    
    char *ret = new char[total + 1];
    ret[total] = 0;
    
    int locate = 0;
    vector<buffer_struct>::iterator i = values.begin();
    while (i != values.end()) {
        memcpy(ret + locate, (*i).buffer, (*i).size);
        locate += (*i).size;
        delete [] (*i).buffer;
        i = values.erase(i);
    }
    //cout << ret << endl;
    return ret;
}
