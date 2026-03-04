#ifndef FILE_H
#define FILE_H

#include "originfile.h"

class File : public OriginFile
{
public:
    string content;

    File(string name);
    bool isDirectory();
};

#endif
