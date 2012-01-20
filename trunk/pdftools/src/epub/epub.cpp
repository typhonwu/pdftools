#include "epub.h"
#include <zip.h>

EPUB::EPUB() : Generator()
{

}

void EPUB::generate(Document* document, const char* output)
{
    int errors = 0;
    zip *zipfile = zip_open(output, ZIP_CREATE, &errors);
    if (zipfile) {
        struct zip_source *s;

        if ((s = zip_source_buffer(zipfile, "teste", 5, 0)) == NULL ||
                zip_add(zipfile, "META-INF/teste", s) < 0) {
            zip_source_free(s);
            printf("error adding file: %sn", zip_strerror(zipfile));
        }

        zip_close(zipfile);
    }
}
