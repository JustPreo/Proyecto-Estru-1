#include "originfile.h"
#include <ctime>

OriginFile::OriginFile(string name) {
    this->name= name;
    this->isFavorite= false;
    this->originalPath = "";
    this->parent = NULL;

    time_t now = time(NULL);

    createdDate = now;
    modifiedDate = now;
}
