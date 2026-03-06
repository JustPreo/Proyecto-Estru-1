#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "originfile.h"
#include <vector>

using std::vector;

class Directory : public OriginFile
{
public:
    vector<OriginFile*> children;
    Directory(string name);
    bool isDirectory();
    ~Directory();
};

#endif
