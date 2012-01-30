#include "epub.h"
#include "utils.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <libxml/xmlwriter.h>

using namespace std;

//rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "X_ORDER_ID", "%010d", 53535);

#define CHECK_ERROR(x) if (x < 0) {return;}

EPUB::EPUB() : Generator()
{
    m_zipfile = NULL;
}

EPUB::~EPUB()
{
}

void EPUB::generate_mimetype()
{
    const char *mime = "application/epub+zip";
    struct zip_source *s;
    if ((s = zip_source_buffer(m_zipfile, mime, strlen(mime), false)) == NULL ||
            zip_add(m_zipfile, "mimetype", s) < 0) {
        error_message(zip_strerror(m_zipfile));
    }
}

void EPUB::generate_container()
{
    xmlTextWriterPtr writer;

    xmlBufferPtr buffer = xmlBufferCreate();
    if (buffer == NULL) {
        return;
    }

    writer = xmlNewTextWriterMemory(buffer, 0);
    if (writer == NULL) {
        return;
    }

    CHECK_ERROR(xmlTextWriterStartDocument(writer, "1.0", "UTF-8", NULL));

    CHECK_ERROR(xmlTextWriterStartElement(writer, BAD_CAST "container"));
    CHECK_ERROR(xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns",
            BAD_CAST "urn:oasis:names:tc:opendocument:xmlns:container"));

    CHECK_ERROR(xmlTextWriterStartElement(writer, BAD_CAST "rootfiles"));

    CHECK_ERROR(xmlTextWriterStartElement(writer, BAD_CAST "rootfile"));
    CHECK_ERROR(xmlTextWriterWriteAttribute(writer, BAD_CAST "full-path", BAD_CAST "content.opf"));
    CHECK_ERROR(xmlTextWriterWriteAttribute(writer, BAD_CAST "media-type",
            BAD_CAST "application/oebps-package+xml"));
    
    CHECK_ERROR(xmlTextWriterEndElement(writer));
    CHECK_ERROR(xmlTextWriterEndElement(writer));
    CHECK_ERROR(xmlTextWriterEndElement(writer));

    xmlFreeTextWriter(writer);

    string content = (const char *) buffer->content;
    xmlBufferFree(buffer);

    struct zip_source *s;
    if ((s = zip_source_buffer(m_zipfile, content.c_str(), content.length(), 0)) == NULL ||
            zip_add(m_zipfile, "META-INF/container.xml", s) < 0) {
        error_message(zip_strerror(m_zipfile));
    }
}

void EPUB::generate_content()
{

}

void EPUB::generate(Document* document, const char* output)
{
    int errors = 0;
    remove(output);
    m_zipfile = zip_open(output, ZIP_CREATE, &errors);
    if (m_zipfile) {
        generate_mimetype();
        generate_container();
        errors = zip_close(m_zipfile);
    } else {
        printf("error adding file: %sn", zip_strerror(m_zipfile));
    }
}
