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

const char *doc_encoding_table[256] = {
    "\x0000", "\x0001", "\x0002", "\x0003", "\x0004", "\x0005", "\x0006", "\x0007", // 00
    "\x0008", "\x0009", "\x000a", "\x000b", "\x000c", "\x000d", "\x000e", "\x000f",
    "\x0010", "\x0011", "\x0012", "\x0013", "\x0014", "\x0015", "\x0016", "\x0017", // 10
    "\u02d8", "\u02c7", "\u02c6", "\u02d9", "\u02dd", "\u02db", "\u02da", "\u02dc",
    "\x0020", "\x0021", "\x0022", "\x0023", "\x0024", "\x0025", "\x0026", "\x0027", // 20
    "\x0028", "\x0029", "\x002a", "\x002b", "\x002c", "\x002d", "\x002e", "\x002f",
    "\x0030", "\x0031", "\x0032", "\x0033", "\x0034", "\x0035", "\x0036", "\x0037", // 30
    "\x0038", "\x0039", "\x003a", "\x003b", "\x003c", "\x003d", "\x003e", "\x003f",
    "\x0040", "\x0041", "\x0042", "\x0043", "\x0044", "\x0045", "\x0046", "\x0047", // 40
    "\x0048", "\x0049", "\x004a", "\x004b", "\x004c", "\x004d", "\x004e", "\x004f",
    "\x0050", "\x0051", "\x0052", "\x0053", "\x0054", "\x0055", "\x0056", "\x0057", // 50
    "\x0058", "\x0059", "\x005a", "\x005b", "\x005c", "\x005d", "\x005e", "\x005f",
    "\x0060", "\x0061", "\x0062", "\x0063", "\x0064", "\x0065", "\x0066", "\x0067", // 60
    "\x0068", "\x0069", "\x006a", "\x006b", "\x006c", "\x006d", "\x006e", "\x006f",
    "\x0070", "\x0071", "\x0072", "\x0073", "\x0074", "\x0075", "\x0076", "\x0077", // 70
    "\x0078", "\x0079", "\x007a", "\x007b", "\x007c", "\x007d", "\x007e", "\ufffd",
    "\u2022", "\u2020", "\u2021", "\u2026", "\u2014", "\u2013", "\u0192", "\u2044", // 80
    "\u2039", "\u203a", "\u2212", "\u2030", "\u201e", "\u201c", "\u201d", "\u2018",
    "\u2019", "\u201a", "\u2122", "\ufb01", "\ufb02", "\u0141", "\u0152", "\u0160", // 90
    "\u0178", "\u017d", "\u0131", "\u0142", "\u0153", "\u0161", "\u017e", "\ufffd",
    "\u20ac", "\x00a1", "\x00a2", "\x00a3", "\x00a4", "\x00a5", "\x00a6", "\x00a7", // a0
    "\x00a8", "\x00a9", "\x00aa", "\x00ab", "\x00ac", "\ufffd", "\x00ae", "\x00af",
    "\x00b0", "\x00b1", "\x00b2", "\x00b3", "\x00b4", "\x00b5", "\x00b6", "\x00b7", // b0
    "\x00b8", "\x00b9", "\x00ba", "\x00bb", "\x00bc", "\x00bd", "\x00be", "\x00bf",
    "\x00c0", "\x00c1", "\x00c2", "\x00c3", "\x00c4", "\x00c5", "\x00c6", "\x00c7", // c0
    "\x00c8", "\x00c9", "\x00ca", "\x00cb", "\x00cc", "\x00cd", "\x00ce", "\x00cf",
    "\x00d0", "\x00d1", "\x00d2", "\x00d3", "\x00d4", "\x00d5", "\x00d6", "\x00d7", // d0
    "\x00d8", "\x00d9", "\x00da", "\x00db", "\x00dc", "\x00dd", "\x00de", "\x00df",
    "\x00e0", "\x00e1", "\x00e2", "\x00e3", "\x00e4", "\x00e5", "\x00e6", "\x00e7", // e0
    "\x00e8", "\x00e9", "\x00ea", "\x00eb", "\x00ec", "\x00ed", "\x00ee", "\x00ef",
    "\x00f0", "\x00f1", "\x00f2", "\x00f3", "\x00f4", "\x00f5", "\x00f6", "\x00f7", // f0
    "\x00f8", "\x00f9", "\x00fa", "\x00fb", "\x00fc", "\x00fd", "\x00fe", "\x00ff"
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
    int err = deflateInit(&zstream, Z_BEST_COMPRESSION);
    if (err != Z_OK) {
        return NULL;
    }

    zstream.avail_in = size;
    zstream.next_in = (Bytef *) raw;

    register int total = 0;
    do {
        buffer_struct b;
        zstream.avail_out = MAX_BUFFER_SIZE;
        zstream.next_out = (Bytef *) b.buffer;

        err = deflate(&zstream, Z_FINISH);
        b.size = MAX_BUFFER_SIZE - zstream.avail_out;

        total += b.size;
        values.push_back(b);
    } while (zstream.avail_out == 0);

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

#include <errno.h>

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
            // Handle failures.
            if ((int) iconv_value != -1) {
                ret = utf8start;
            }
            delete [] utf8start;
            iconv_close(conv_desc);
        }
    } else {
        string converted;
        int size = str.length();

        for (int loop = 0; loop < size; loop++) {
            uint8_t c = str[loop];

            const char *new_char = doc_encoding_table[c];
            converted += new_char;
        }
        return converted;
    }
    return ret;
}
