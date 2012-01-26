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

#define MAX_BUFFER_SIZE 2048

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

char *flat_decode(int8_t *compressed, int size)
{
    vector<buffer_struct> values;

    z_stream zstream;
    memset(&zstream, 0, sizeof (z_stream));
    zstream.avail_in = size;
    zstream.next_in = (Bytef *) compressed;

    int total = 0;
    int rsti = inflateInit(&zstream);
    if (rsti == Z_OK) {
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
        i++;
    }
    return ret;
}
