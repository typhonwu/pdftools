#include "epub.h"
#include <cstring>
#include <iostream>

using namespace std;

static const char *container_string = "<?xml version=\"1.0\"?><container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">"
        "<rootfiles><rootfile full-path=\"content.opf\" media-type=\"application/oebps-package+xml\"/></rootfiles></container>";

static const char *mime_type = "application/epub+zip";

EPUB::EPUB() : Generator()
{
    m_zipfile = NULL;
}

EPUB::~EPUB()
{
}

void EPUB::generate_mimetype()
{
    struct zip_source *s;
    if ((s = zip_source_buffer(m_zipfile, mime_type, strlen(mime_type), 0)) == NULL ||
            zip_add(m_zipfile, "mimetype", s) < 0) {
        zip_source_free(s);
    }
}

void EPUB::generate_container()
{
    struct zip_source *s;
    if ((s = zip_source_buffer(m_zipfile, container_string, strlen(container_string), 0)) == NULL ||
            zip_add(m_zipfile, "META-INF/container.xml", s) < 0) {
        zip_source_free(s);
        //printf("error adding file: %sn", zip_strerror(m_zipfile));
    }

}

void EPUB::generate(Document* document, const char* output)
{
    int errors = 0;
    m_zipfile = zip_open(output, ZIP_CREATE, &errors);
    if (m_zipfile) {
        generate_mimetype();
        generate_container();
        errors = zip_close(m_zipfile);
    }
}
