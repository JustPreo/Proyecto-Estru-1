#include "directory.h"

Directory::Directory(string name)
    : OriginFile(name)
{
}

bool Directory::isDirectory()
{
    return true;
}

Directory::~Directory()
{
    for (int i = 0; i < children.size(); i++)
    {
        delete children[i];
    }
}
