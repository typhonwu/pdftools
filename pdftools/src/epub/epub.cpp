#include "epub.h"

#include "utils.h"
#include <cstring>
#include <iostream>
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

void EPUB::generate_content()
{

}

bool EPUB::generate(Document* document, const char* output)
{
//    
//    Html html;
//    html.start_document();
//    html.start_header();
//    html.set_title("teste de título");
//    html.end_tag();
//    
//    html.start_body();
//        
//    html.end_tag();
//    html.end_document();
//    cout << html.content() << endl;
    

    if (m_zipfile->open(output)) {
        generate_mimetype();
        generate_container();
        //m_zipfile->add_source("teste.txt", "teste de texto\x0a");
        m_zipfile->close();
    } else {
        return false;
    }
    return true;
}
