#include "epub.h"
#include "config.h"
#include "utils.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include "xml/xml.h"
#include "html/html.h"

using namespace std;

EPUB::EPUB() : Generator()
{
    m_zipfile = new ZipFile;
}

EPUB::~EPUB()
{
    if (m_zipfile) delete m_zipfile;
}

void EPUB::generate_mimetype()
{
    const char *mime = "application/epub+zip";
    m_zipfile->add_source("mimetype", mime);
}

void EPUB::generate_container()
{
    XML xml;
    xml.start_document("1.0", "UTF-8");

    xml.start_tag("container");
    xml.add_attribute("version", "1.0");
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
    m_zipfile->add_source("META-INF/container.xml", content.c_str());
}

void EPUB::generate_content(const char* output)
{
    XML xml;
    xml.start_document("1.0", "UTF-8");

    xml.start_tag("package");
    xml.add_attribute("xmlns", "http://www.idpf.org/2007/opf");
    xml.add_attribute("unique-identifier", "dcidid");
    xml.add_attribute("version", "2.0");

    xml.start_tag("metadata");
    xml.add_attribute("xmlns:dc", "http://purl.org/dc/elements/1.1/");
    xml.add_attribute("xmlns:dcterms", "http://purl.org/dc/terms/");
    xml.add_attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    xml.add_attribute("xmlns:opf", "http://www.idpf.org/2007/opf");

    xml.start_tag("dc:title");
    xml.add_element(m_document->title().c_str());
    xml.end_tag();

    xml.start_tag("dc:language");
    xml.add_attribute("xsi:type", "dcterms:RFC3066");
    xml.add_element(m_document->lang().c_str());
    xml.end_tag();

    xml.start_tag("dc:identifier");
    xml.add_attribute("id", "dcidid");
    xml.add_attribute("opf:scheme", "URI");
    xml.add_element(output);
    xml.end_tag();

    xml.start_tag("dc:subject");
    xml.add_element(m_document->subject().c_str());
    xml.end_tag();

    xml.start_tag("dc:relation");
    xml.add_element(PACKAGE_URL);
    xml.end_tag();

    xml.start_tag("dc:creator");
    if (m_document->author().empty()) {
        xml.add_element("no title");
    } else {
        xml.add_element(m_document->author().c_str());
    }
    xml.end_tag();

    xml.start_tag("dc:publisher");
    xml.add_element(PACKAGE_STRING);
    xml.end_tag();

    xml.start_tag("dc:publisher");
    xml.add_element(m_document->author().c_str());
    xml.end_tag();

    xml.end_tag();

    int i;
    int size = m_document->pages();
    xml.start_tag("manifest");

    xml.start_tag("item");
    xml.add_attribute("id", "ncx");
    xml.add_attribute("href", "toc.ncx");
    xml.add_attribute("media-type", "application/x-dtbncx+xml");
    xml.end_tag();

    for (i = 1; i <= size; i++) {
        stringstream part;
        part << "part" << i;
        stringstream file;
        file << "page-" << i << ".html";

        xml.start_tag("item");
        xml.add_attribute("id", part.str().c_str());
        xml.add_attribute("href", file.str().c_str());
        xml.add_attribute("media-type", "application/xhtml+xml");
        xml.end_tag();
    }
    xml.end_tag();

    xml.start_tag("spine");
    xml.add_attribute("toc", "ncx");
    for (i = 1; i <= size; i++) {
        stringstream part;
        part << "part" << i;

        xml.start_tag("itemref");
        xml.add_attribute("idref", part.str().c_str());
        xml.end_tag();
    }
    xml.end_tag();

    xml.end_tag();
    xml.end_document();
    string content = xml.content();
    m_zipfile->add_source("content.opf", content.c_str());
}

void EPUB::generate_toc(const char* output)
{
    XML xml;
    xml.start_document("1.0", "UTF-8");

    xml.start_tag("ncx");
    xml.add_attribute("xmlns", "http://www.daisy.org/z3986/2005/ncx/");
    xml.add_attribute("version", "2005-1");

    xml.start_tag("head");
    xml.start_tag("meta");
    xml.add_attribute("name", "dtb:uid");
    xml.add_attribute("content", output);
    xml.end_tag();
    xml.start_tag("meta");
    xml.add_attribute("name", "dtb:depth");
    xml.add_attribute("content", "1");
    xml.end_tag();
    xml.start_tag("meta");
    xml.add_attribute("name", "dtb:totalPageCount");
    xml.add_attribute("content", "0");
    xml.end_tag();
    xml.start_tag("meta");
    xml.add_attribute("name", "dtb:maxPageNumber");
    xml.add_attribute("content", "0");
    xml.end_tag();
    xml.end_tag();

    xml.start_tag("docTitle");
    xml.start_tag("text");
    xml.add_element(m_document->title().c_str());
    xml.end_tag();
    xml.end_tag();

    int i;
    int size = m_document->pages();
    xml.start_tag("navMap");
    for (i = 1; i <= size; i++) {
        stringstream id;
        id << "navPoint-" << i;
        stringstream file;
        file << "page-" << i << ".html";
        stringstream playorder;
        playorder << i;

        xml.start_tag("navPoint");
        xml.add_attribute("id", id.str().c_str());
        xml.add_attribute("playOrder", playorder.str().c_str());

        xml.start_tag("navLabel");
        xml.start_tag("text");
        xml.add_element("Page title");
        xml.end_tag();
        xml.end_tag();

        xml.start_tag("content");
        xml.add_attribute("src", file.str().c_str());
        xml.end_tag();

        xml.end_tag();
    }
    xml.end_tag();

    xml.end_tag();
    xml.end_document();
    string content = xml.content();
    m_zipfile->add_source("toc.ncx", content.c_str());
}

void EPUB::generate_page(Page *page, const char *filename)
{

    Html html;
    html.start_document();
    html.start_header();
    html.set_title("teste de título");
    html.end_tag();

    html.start_body();
    
    html.add_paragraph();
    html.add_element(filename);
    html.end_tag();

    html.end_tag();
    html.end_document();

    m_zipfile->add_source(filename, html.content());
}

bool EPUB::generate(Document* document, const char* output)
{
    m_document = document;
    if (m_zipfile->open(output)) {
        generate_mimetype();
        generate_container();
        generate_content(output);
        generate_toc(output);

        int i;
        int size = m_document->pages();
        for (i = 1; i <= size; i++) {
            stringstream file;
            file << "page-" << i << ".html";
            generate_page(m_document->page(i), file.str().c_str());
        }

        m_zipfile->close();
    } else {
        return false;
    }
    return true;
}
