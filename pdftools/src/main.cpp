#include <iostream>

#if HAVE_CONFIG_H
#include <config.h>
#else
#error the config.h file is mandatory
#endif

#include "utils.h"
#include "converter.h"
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
    const char *format = "epub";

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
            cout << "Copyright (C) 2012 Leonardo Alves da Costa." << endl;
            cout << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>" << endl;
            cout << "This is free software: you are free to change and redistribute it." << endl;
            cout << "There is NO WARRANTY, to the extent permitted by law." << endl;
            cout << endl;
            cout << "Written by Leonardo Alves da Costa." << endl;
            return EXIT_SUCCESS;
        case 'f':
            format = optarg;
            if (strcmp(format, "azw") && strcmp(format, "epub")) {
                string msg = "format ";
                msg += format;
                msg += " not supported.";
                error_message(msg);
                error = true;
            }
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
        error_message("no input file");
        error = true;
    }
    if (error) {
        cout << "Try `" << PACKAGE_NAME << " --help' for usage." << endl;
        return -1;
    } else {
        while (optind < argc) {
            Converter converter(argv[optind], format);
            converter.convert();
            optind++;
            if (optind < argc && verbose_mode()) {
                verbose_message("");
            }
        }
    }
    return EXIT_SUCCESS;
}
