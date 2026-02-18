#include "filesystem.h"
#include <qdir.h>
#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QDir>
FileSystem::FileSystem() {
    root = new Node("/", NodeType::Folder, nullptr);
    current = root;
}

FileSystem::~FileSystem() {
    delete root; // borra todo lo de adentro
}

Node* FileSystem::getRoot() {
    return root;
}

Node* FileSystem::getCurrent() {
    return current;
}

void FileSystem::changeDirectory(Node* target) {
    if (target != nullptr && target->isFolder()) {
        current = target;
    }
}

Node* FileSystem::createFolder(QString name) {
    Node* folder = new Node(name, NodeType::Folder, current);
    current->children.append(folder);
    return folder;
}

Node* FileSystem::createFile(QString name) {
    Node* file = new Node(name, NodeType::File, current);
    current->children.append(file);
    return file;
}

void FileSystem::saveToBin()
{
    QString basePath = QCoreApplication::applicationDirPath();
    QString dataPath = basePath + "/data";

    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString filePath = dataPath + "/filesystem.bin";

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);

    saveNode(out, root);

    file.close();
}

void FileSystem::saveNode(QDataStream &out, Node* node)
{
    out << node->name;
    out << (int)node->type;
    out << node->children.size();

    if (node->isFile()) {
        out << node->content;
    }

    for (int i = 0; i < node->children.size(); i++) {
        Node* child = node->children[i];
        saveNode(out, child);
    }
}


void FileSystem::loadFromBin()
{
    QString filePath =
        QCoreApplication::applicationDirPath() + "/data/filesystem.bin";

    QFile file(filePath);

    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadOnly))
        return;

    delete root;

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);

    root = loadNode(in, nullptr);
    current = root;

    file.close();
}
Node* FileSystem::loadNode(QDataStream &in, Node* parent)
{
    QString name;
    int typeInt;
    int childCount;

    in >> name;
    in >> typeInt;
    in >> childCount;

    Node* node = new Node(name, (NodeType)typeInt, parent);

    if (node->isFile()) {
        in >> node->content;
    }

    for (int i = 0; i < childCount; i++) {
        Node* child = loadNode(in, node);
        node->children.append(child);
    }

    return node;
}
