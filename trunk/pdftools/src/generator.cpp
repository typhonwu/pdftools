#include "generator.h"
#include "semantic/document.h"
#include "epub/epub.h"
#include <cstdlib>
#include <cstring>

Generator::Generator()
{
}

Generator::~Generator()
{
}

Generator *Generator::get_instance(const string& format)
{
    if (format == "epub") {
        return new EPUB;
    }
    return nullptr;
}
