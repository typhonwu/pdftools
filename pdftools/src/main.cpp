#include <iostream>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "parser.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3) {
        cout << "Use " << argv[0] << " filein fileout" << endl;
    } else {
        Parser parser;
        if (parser.open_file(argv[1])) {
            parser.parse();
        } else {
            cerr << "File " << argv[1] << " could not be opened." << endl;
        }
    }
    return EXIT_SUCCESS;
}
