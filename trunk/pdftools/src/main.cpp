#include <iostream>

#if HAVE_CONFIG_H
#include <config.h>
#else
#error the config.h file is mandatory
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

int main(int argc, char *argv[])
{
    int c;
    int verbose_flag = 0;
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
            wcout << L"Usage: " << argv[0] << L" [options] file..." << endl;
            wcout << L"Options:" << endl;
            wcout << L"  -f <format>, --format=<format>\toutput file format [epub]" << endl;
            wcout << L"  -h\t\t\t\t\tdisplay this information and quit" << endl;
            wcout << L"  -o <file>\t\t\t\tset the output file" << endl;
            wcout << L"  -v, --version\t\t\t\tdisplay the version information" << endl;
            wcout << L"  --verbose\t\t\t\trun in verbose mode" << endl;
            wcout << endl;            
            wcout << L"Report bugs to " << PACKAGE_BUGREPORT << endl;
            wcout << PACKAGE_NAME << L" home page: <" << PACKAGE_URL << L">" << endl;
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
            wcout << format << endl;
            break;
        case '?':
            error = true;
            break;
        }
    }

    if (verbose_flag) {
        set_verbose_mode(true);
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
