#ifndef ORIGINFILE_H
#define ORIGINFILE_H

#include <string>
using std::string;

class OriginFile {
public:
    string name;
    bool isFavorite;
    string originalPath;
    OriginFile* parent;
    OriginFile(string name);
    virtual ~OriginFile() {}

    virtual bool isDirectory() = 0;//si o si ocupo tenerlo
};

#endif
