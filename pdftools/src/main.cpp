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
    const char *format = NULL;

    set_program_name(to_unicode(argv[0]));

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
            wcout << PACKAGE_NAME << L" is a high level tools to convert PDF files to AZW or ePUB formats." << endl;
            wcout << endl;
            wcout << L"Usage: " << argv[0] << L" [options] file..." << endl;
            wcout << L"Options:" << endl;
            wcout << L"  -f <format>, --format=<format>\toutput file format [epub]" << endl;
            wcout << L"  -h, --help\t\t\t\tdisplay this information and quit" << endl;
            wcout << L"  -o <file>\t\t\t\tset the output file" << endl;
            wcout << L"  -v, --version\t\t\t\tdisplay the version information" << endl;
            wcout << L"  --verbose\t\t\t\trun in verbose mode" << endl;
            wcout << endl;
            wcout << L"Report bugs to " << PACKAGE_BUGREPORT << endl;
            wcout << PACKAGE_NAME << L" home page: <http://code.google.com/p/pdftools/issues/list>" << endl;
            return EXIT_SUCCESS;
        case 'v':
            wcout << PACKAGE_STRING << endl;
            wcout << endl;
            wcout << L"Copyright (C) 2012 Leonardo Alves da Costa." << endl;
            wcout << L"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>" << endl;
            wcout << L"This is free software: you are free to change and redistribute it." << endl;
            wcout << L"There is NO WARRANTY, to the extent permitted by law." << endl;
            wcout << endl;
            wcout << L"Written by Leonardo Alves da Costa." << endl;
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

    if (optind >= argc && !error) {
        error_message(L"no input file");
        error = true;
    }
    if (error) {
        wcout << L"Try `" << argv[0] << L" --help' for usage." << endl;
        return -1;
    } else {
        while (optind < argc) {
            if (verbose_mode()) {
                wstring msg = L"parsing file ";
                msg += to_unicode(argv[optind]);
                verbose_message(msg.c_str());
            }
            Parser parser;
            if (!parser.open_file(argv[optind])) {
                error_message(L"file not found");
            } else {
                parser.parse();
            }
            optind++;
        }
    }
    return EXIT_SUCCESS;
}
