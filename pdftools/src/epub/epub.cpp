#include "epub.h"
#include <cstring>
#include <iostream>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

using namespace std;

static const char *container_string = "<?xml version=\"1.0\"?><container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">"
        "<rootfiles><rootfile full-path=\"content.opf\" media-type=\"application/oebps-package+xml\"/></rootfiles></container>";

static const char *mime_type = "application/epub+zip";

/**
 * ConvertInput:
 * @in: string in a given encoding
 * @encoding: the encoding used
 *
 * Converts @in into UTF-8 for processing with libxml2 APIs
 *
 * Returns the converted UTF-8 string, or NULL in case of error.
 */
xmlChar *
ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;
}

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
    LIBXML_TEST_VERSION
    
    int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;
    xmlChar *tmp;
    //FILE *fp;

    /* Create a new XML buffer, to which the XML document will be
     * written */
    buf = xmlBufferCreate();
    if (buf == NULL) {
        printf("testXmlwriterMemory: Error creating the xml buffer\n");
        return;
    }

    /* Create a new XmlWriter for memory, with no compression.
     * Remark: there is no compression for this kind of xmlTextWriter */
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL) {
        printf("testXmlwriterMemory: Error creating the xml writer\n");
        return;
    }

    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartDocument\n");
        return;
    }

    /* Start an element named "EXAMPLE". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "EXAMPLE");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write a comment as child of EXAMPLE.
     * Please observe, that the input to the xmlTextWriter functions
     * HAS to be in UTF-8, even if the output XML is encoded
     * in iso-8859-1 */
    tmp = ConvertInput("This is a comment with special chars: <äöü>",
                       "UTF-8");
    rc = xmlTextWriterWriteComment(writer, tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteComment\n");
        return;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Start an element named "ORDER" as child of EXAMPLE. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ORDER");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Add an attribute with name "version" and value "1.0" to ORDER. */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version",
                                     BAD_CAST "1.0");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteAttribute\n");
        return;
    }

    /* Add an attribute with name "xml:lang" and value "de" to ORDER. */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xml:lang",
                                     BAD_CAST "de");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteAttribute\n");
        return;
    }

    /* Write a comment as child of ORDER */
    tmp = ConvertInput("<äöü>", "UTF-8");
    rc = xmlTextWriterWriteFormatComment(writer,
		     "This is another comment with special chars: %s",
                                         tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatComment\n");
        return;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Start an element named "HEADER" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "HEADER");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write an element named "X_ORDER_ID" as child of HEADER. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "X_ORDER_ID",
                                         "%010d", 53535);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Write an element named "CUSTOMER_ID" as child of HEADER. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "CUSTOMER_ID",
                                         "%d", 1010);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Write an element named "NAME_1" as child of HEADER. */
    tmp = ConvertInput("Müller", "UTF-8");
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "NAME_1", tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Write an element named "NAME_2" as child of HEADER. */
    tmp = ConvertInput("Jörg", "UTF-8");
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "NAME_2", tmp);

    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Close the element named HEADER. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Start an element named "ENTRIES" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRIES");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Start an element named "ENTRY" as child of ENTRIES. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRY");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write an element named "ARTICLE" as child of ENTRY. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "ARTICLE",
                                   BAD_CAST "<Test>");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return;
    }

    /* Write an element named "ENTRY_NO" as child of ENTRY. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ENTRY_NO", "%d",
                                         10);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Close the element named ENTRY. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Start an element named "ENTRY" as child of ENTRIES. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRY");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write an element named "ARTICLE" as child of ENTRY. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "ARTICLE",
                                   BAD_CAST "<Test 2>");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return;
    }

    /* Write an element named "ENTRY_NO" as child of ENTRY. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ENTRY_NO", "%d",
                                         20);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Close the element named ENTRY. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Close the element named ENTRIES. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Start an element named "FOOTER" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "FOOTER");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write an element named "TEXT" as child of FOOTER. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "TEXT",
                                   BAD_CAST "This is a text.");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return;
    }

    /* Close the element named FOOTER. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndDocument\n");
        return;
    }

    xmlFreeTextWriter(writer);

    cout << (const char *) buf->content << endl;
//    fp = fopen(file, "w");
//    if (fp == NULL) {
//        printf("testXmlwriterMemory: Error at fopen\n");
//        return;
//    }
//
//    fprintf(fp, "%s", (const char *) buf->content);
//
//    fclose(fp);

    xmlBufferFree(buf);
    
    int errors = 0;
    m_zipfile = zip_open(output, ZIP_CREATE, &errors);
    if (m_zipfile) {
        generate_mimetype();
        generate_container();
        errors = zip_close(m_zipfile);
    }
}
