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

static int verbose_flag = 0;

int main(int argc, char *argv[])
{
    int c;
    const char *fileout = NULL;
    const char *filein = NULL;

    set_program_name(argv[0]);

    while (true) {
        static struct option long_options[] = {
            /* These options set a flag. */
            {"verbose", no_argument, &verbose_flag, 1},
            {"help", no_argument, 0, 'h'},
            {"o", required_argument, 0, 'o'},
            {"format", required_argument, 0, 'f'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "h:o:f:",
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
            printf("help\n");
            break;

        case 'f':
            printf("option -f with value `%s'\n", optarg);
            break;

        case '?':
            /* getopt_long already printed an error message. */
            return -1;
            break;

        default:
            abort();
        }
    }

    //if (verbose_flag)
    //    puts("verbose flag is set");

    if (optind < argc) {
        wstring msg = L"unknow option";
        filein = argv[optind++];
    }
    if (optind < argc) {
        error_message(L"can't parse more than one input file");
    } else if (!filein) {
        error_message(L"no input file");
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
