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
    vector<char *> values;

    // 2k buffer, last byte is for \0 (end string)
    char buffer[2049];
    z_stream zstream;
    memset(&zstream, 0, sizeof (z_stream));

    zstream.avail_in = size;
    zstream.avail_out = sizeof (buffer) - 1;
    zstream.next_in = (Bytef *) compressed;
    zstream.next_out = (Bytef *) buffer;

    int count = 0;
    int rsti = inflateInit(&zstream);
    if (rsti == Z_OK) {
        do {
            count++;
            zstream.avail_out = sizeof (buffer) - 1;
            memset(buffer, 0, sizeof (buffer));
            zstream.next_out = (Bytef *) buffer;

            int rst2 = inflate(&zstream, Z_NO_FLUSH);
            if (rst2 >= 0) {
                char *temp = new char[zstream.total_out + 1];
                memcpy(temp, buffer, zstream.total_out);
                temp[zstream.total_out] = 0;
                //cout << strlen(buffer) << endl << strlen(temp) << endl;
                values.push_back(temp);
                if (rst2 == Z_STREAM_END) break;
            } else {
                cout << "error" << endl;
                // Error in decompression
                break;
            }
        } while (zstream.avail_out == 0);
    }
    
    char *ret = new char[values.size() + 1];
    ret[values.size()] = 0;
    
    memcpy(ret, values[0], values.size());
    vector<char *>::iterator i = values.begin();
    while (i != values.end()) {
        cout << *i << endl;
        delete [] (*i);
        i = values.erase(i);
    }
    inflateEnd(&zstream);
    
    return ret;
}
