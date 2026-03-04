#ifndef ORIGINFILE_H
#define ORIGINFILE_H

#include <string>
using std::string;

class OriginFile {
public:
    string name;
    bool isFavorite;
    OriginFile(string name);
    virtual ~OriginFile() {}

    virtual bool isDirectory() = 0;//obligatorio
};

#endif
