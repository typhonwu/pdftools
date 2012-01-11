#include <iostream>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "utils.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <getopt.h>

using namespace std;

int verbose_flag = 0;

int main(int argc, char *argv[])
{
    int c;
    bool error = false;
    const char *fileout = NULL;
    const char *filein = NULL;
    const char *format = NULL;

    set_program_name(argv[0]);

    while (true) {
        static struct option long_options[] = {
            {"verbose", no_argument, &verbose_flag, 1},
            {"help", no_argument, 0, 'h'},
            {"version", no_argument, 0, 'v'},
            {"o", required_argument, 0, 'o'},
            {"format", required_argument, 0, 'f'},
            {0, 0, 0, 0}
        };
        int option_index = 0;

        c = getopt_long(argc, argv, "hvo:f:",
                long_options, &option_index);

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
            printf("help 2\n");
            return EXIT_SUCCESS;
        case 'v':
            wcout << PACKAGE_STRING << endl;
            wcout << "Copyright (C) 2007 Free Software Foundation, Inc." << endl;
            wcout << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>" << endl;
            wcout << "This is free software: you are free to change and redistribute it." << endl;
            wcout << "There is NO WARRANTY, to the extent permitted by law." << endl;
            return EXIT_SUCCESS;
        case 'f':
            format = optarg;
            break;
        case '?':
            error = true;
            break;
        default:
            error = true;
            error_message(L"internal error");
        }
    }

    //if (verbose_flag)
    //    puts("verbose flag is set");

    if (optind < argc) {
        wstring msg = L"unknow option";
        filein = argv[optind++];
        error = true;
    }
    if (optind < argc) {
        error_message(L"can't parse more than one input file");
        error = true;
    } else if (!filein && !error) {
        error_message(L"no input file");
        error = true;
    }

    if (error) {
        wcout << L"Try `" << argv[0] << L" --help' for usage." << endl;
        return -1;
    } else {
        Parser parser;
        if (!parser.open_file(filein)) {
            error_message(L"file not found");
        } else {
            parser.parse();
        }
    }
    return EXIT_SUCCESS;
}
