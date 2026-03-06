#include "originfile.h"

OriginFile::OriginFile(string name) {
    this->name= name;
    this->isFavorite= false;
    this->originalPath = "";
    this->parent = NULL;
}
