#include "HistoryList.h"

HistoryList::HistoryList()
{
    head = nullptr;
    tail = nullptr;
    current = nullptr;
}

void HistoryList::visit(Node* location)
{
    HistoryNode* node = new HistoryNode(location);

    if (head == nullptr) {
        head = tail = current = node;//el sugar syntax que puso el mr
        return;
    }

    // Si estaba en medio, corta el forward
    if (current->next != nullptr) {
        HistoryNode* temp = current->next;
        while (temp != nullptr) {
            HistoryNode* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
        current->next = nullptr;
        tail = current;
    }

    current->next = node;
    node->prev = current;
    current = node;
    tail = node;
}
//Funcioines auxiliars
bool HistoryList::canGoBack()
{
    return current != nullptr && current->prev != nullptr;
}

bool HistoryList::canGoForward()
{
    return current != nullptr && current->next != nullptr;
}

Node* HistoryList::getCurrent()
{
    if (current == nullptr)
        return nullptr;

    return current->location;
}

Node* HistoryList::goBack()
{
    if (!canGoBack())
        return nullptr;

    current = current->prev;
    return current->location;
}

Node* HistoryList::goForward()
{
    if (!canGoForward())
        return nullptr;

    current = current->next;
    return current->location;
}


