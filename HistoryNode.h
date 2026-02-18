#ifndef HISTORYNODE_H
#define HISTORYNODE_H

#include "node.h"
#include <QString>

class HistoryNode {
public:
    Node* location;
    HistoryNode* prev;
    HistoryNode* next;

    HistoryNode(Node* location);
};

#endif
