#include "HistoryList.h"


HistoryList::HistoryList()
{
    current = NULL;
}


void HistoryList::visit(Directory* dir)
{
    HistoryNode* node = new HistoryNode(dir);

    if(current == NULL)
    {
        current = node;
        return;
    }

    node->prev = current;

    current->next = node;

    current = node;
}

Directory* HistoryList::goBack()
{
    if(current == NULL) return NULL;

    if(current->prev == NULL) return NULL;

    current = current->prev;

    return current->dir;
}

Directory* HistoryList::goForward()
{
    if(current == NULL) return NULL;

    if(current->next == NULL) return NULL;

    current = current->next;

    return current->dir;
}

bool HistoryList::canGoBack()
{
    if(current == NULL) return false;

    return current->prev != NULL;
}

bool HistoryList::canGoForward()
{
    if(current == NULL) return false;

    return current->next != NULL;
}
