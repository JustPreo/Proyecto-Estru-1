#include "HistoryNode.h"

HistoryNode::HistoryNode(Node* location)
{
    this->location = location;
    this->prev = nullptr;
    this->next = nullptr;
}
