#include <iostream>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "parser.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <getopt.h>

using namespace std;

static int verbose_flag = 0;

static wchar_t *program_name = NULL;

void set_program_name(char *name)
{
    program_name = new wchar_t[strlen(name) + 1];
    mbstowcs(program_name, name, strlen(name));
}

void error_message(const wchar_t *msg)
{
    wcerr << program_name << L": " << msg << endl;
}

int main(int argc, char *argv[])
{
    int c;
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
                filein = optarg;
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

    /* Instead of reporting ‘--verbose’
       and ‘--brief’ as they are encountered,
       we report the final status resulting from them. */
    if (verbose_flag)
        puts("verbose flag is set");

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        wstring msg = L"unknow option";

        //error_message()
        //printf("non-option ARGV-elements: ");
        while (optind < argc) {
            wchar_t *temp = new wchar_t[strlen(argv[optind]) + 1];
            mbstowcs(temp, argv[optind], strlen(argv[optind]));
            msg += L" ";
            //msg += temp;
            optind++;
        }
        error_message(msg.c_str());
    }

    /*
 if (argc < 3) {
     cout << "Use " << argv[0] << " filein fileout" << endl;
 } else {
     Parser parser;
     if (parser.open_file(argv[1])) {
         parser.parse();
     } else {
         cerr << "File " << argv[1] << " could not be opened." << endl;
     }
 }*/
    return EXIT_SUCCESS;
}
