#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include "HistoryNode.h"
#include "node.h"

class HistoryList {
private:
    HistoryNode* head;
    HistoryNode* tail;
    HistoryNode* current;

public:
    HistoryList();

    void visit(Node* location);

    bool canGoBack();
    bool canGoForward();

    Node* getCurrent();
    Node* goBack();
    Node* goForward();
};

#endif
