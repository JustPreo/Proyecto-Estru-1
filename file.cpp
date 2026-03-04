#include "file.h"

File::File(string name)
    : OriginFile(name)
{
    content = "";
}

bool File::isDirectory()
{
    return false;
}
