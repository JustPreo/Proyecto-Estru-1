#include "node.h"

Node::Node(QString name, NodeType type, Node* parent) {
    this->name = name;
    this->type = type;
    this->parent = parent;
    this->createdAt = QDateTime::currentDateTime();
    this->modifiedAt = createdAt;
}

Node::~Node()
{
    // borrado en cascada
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

bool Node::isFile()
{
    return type == NodeType::File;
}

bool Node::isFolder()
{
    return type == NodeType::Folder;
}
