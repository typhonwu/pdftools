#include <iostream>

#include "../config.h"

#include "utils.h"
#include "converter.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>

using namespace std;

int main(int argc, char *argv[])
{
    int verbose_flag = 0;
    bool error = false;
    string fileout;
    string format = "epub";

    static struct option long_options[] = {
        {"verbose", no_argument, &verbose_flag, 1},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"o", required_argument, 0, 'o'},
        {"format", required_argument, 0, 'f'},
        {0, 0, 0, 0}
    };
        
    while (true) {
        int option_index = 0;
        auto c = getopt_long(argc, argv, "hvo:f:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 'o':
            if (optarg) {
                fileout = optarg;
            }
            break;
        case 'h':
            cout << PACKAGE_NAME << " is a high level tools to convert PDF files to AZW or ePUB formats." << endl;
            cout << endl;
            cout << "Usage: " << PACKAGE_NAME << " [options] file..." << endl;
            cout << "Options:" << endl;
            cout << "  -f <format>, --format=<format>\toutput file format [epub]" << endl;
            cout << "  -h, --help\t\t\t\tdisplay this information and quit" << endl;
            cout << "  -o <file>\t\t\t\tset the output file" << endl;
            cout << "  -v, --version\t\t\t\tdisplay the version information" << endl;
            cout << "  --verbose\t\t\t\trun in verbose mode" << endl;
            cout << endl;
            cout << "Report bugs to <http://code.google.com/p/pdftools/issues/list>" << endl;
            return EXIT_SUCCESS;
        case 'v':
            cout << PACKAGE_STRING << endl;
            cout << endl;
            cout << "Copyright (C) 2014 Leonardo Alves da Costa." << endl;
            cout << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>" << endl;
            cout << "This is free software: you are free to change and redistribute it." << endl;
            cout << "There is NO WARRANTY, to the extent permitted by law." << endl;
            cout << endl;
            cout << "Written by Leonardo Alves da Costa." << endl;
            return EXIT_SUCCESS;
        case 'f':
            format = optarg;
            if (format == "azw" && format == "epub") {
                string msg = "format ";
                msg += format;
                msg += " not supported.";
                error_message(msg);
                error = true;
                break;
            }
        }
    }

    if (verbose_flag) {
        set_verbose_mode(true);
    }

    if (optind >= argc && !error) {
        error_message("no input file");
        error = true;
    }
    if (error) {
        cout << "Try `" << PACKAGE_NAME << " --help' for usage." << endl;
        return EXIT_FAILURE;
    } else {
        for (int loop = optind; loop < argc; loop++) {
            Converter converter(argv[loop], fileout, format);
            converter.convert();
        }
    }
    return EXIT_SUCCESS;
}
