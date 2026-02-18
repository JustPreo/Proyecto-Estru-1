#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "node.h"
#include <QDataStream>
class FileSystem {
private:
    Node* root;
    Node* current;

    void saveNode(QDataStream &out, Node* node);
    Node* loadNode(QDataStream &in, Node* parent);
public:
    FileSystem();
    ~FileSystem();

    Node* getRoot();
    Node* getCurrent();

    void changeDirectory(Node* target);
    Node* createFolder(QString name);
    Node* createFile(QString name);

    void saveToBin();
    void loadFromBin();
};

#endif
