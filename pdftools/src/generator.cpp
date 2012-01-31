#include "generator.h"
#include "epub/epub.h"
#include <cstdlib>
#include <cstring>

Generator::Generator()
{
}

Generator::~Generator()
{
}

Generator *Generator::get_instance(const char *format)
{
    if (!strcmp("epub", format)) {
        return new EPUB;
    }
    return NULL;
}
