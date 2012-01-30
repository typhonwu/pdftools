#include "epub.h"

#include "utils.h"
#include <cstring>
#include <iostream>
#include "xml/xml.h"

using namespace std;

EPUB::EPUB() : Generator()
{
    m_zipfile = NULL;
}

EPUB::~EPUB()
{
    if (m_zipfile) delete m_zipfile;
}

void EPUB::generate_mimetype()
{
    /*
    const char *mime = "application/epub+zip";
    struct zip_source *s;
    if ((s = zip_source_buffer(m_zipfile, mime, strlen(mime), false)) == NULL ||
            zip_add(m_zipfile, "mimetype", s) < 0) {
        error_message(zip_strerror(m_zipfile));
    }*/
}

void EPUB::generate_container()
{
    XML xml;
    xml.start_document("1.0", "UTF-8");
    xml.start_tag("container");
    xml.add_attribute("xmlns", "urn:oasis:names:tc:opendocument:xmlns:container");
    xml.start_tag("rootfiles");
    xml.start_tag("rootfile");
    xml.add_attribute("full-path", "content.opf");
    xml.add_attribute("media-type", "application/oebps-package+xml");
    xml.end_tag();
    xml.end_tag();
    xml.end_tag();
    xml.end_document();

    string content = xml.content();
/*
    struct zip_source *s;
    if ((s = zip_source_buffer(m_zipfile, content.c_str(), content.length(), 0)) == NULL ||
            zip_add(m_zipfile, "META-INF/container.xml", s) < 0) {
        error_message(zip_strerror(m_zipfile));
    }*/
}

void EPUB::generate_content()
{

}

void EPUB::generate(Document* document, const char* output)
{
    m_zipfile = new ZipFile;
    /*
    m_zipfile = zip_open(output, ZIP_CREATE, &errors);
    if (m_zipfile) {
        generate_mimetype();
        generate_container();
        errors = zip_close(m_zipfile);
    } else {
        printf("error adding file: %sn", zip_strerror(m_zipfile));
    }*/
}
