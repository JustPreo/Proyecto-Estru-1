#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QDateTime>
#include <QVector>

enum class NodeType {
    File,
    Folder
};

class Node {
public:
    QString name;
    NodeType type;

    QDateTime createdAt;
    QDateTime modifiedAt;
    qint64 size;

    Node* parent;
    QVector<Node*> children;   // solo si es carpeta
    QString content;           // solo si es archivo

    Node(QString name, NodeType type, Node* parent);
    ~Node();

    bool isFile();
    bool isFolder();
};

#endif
