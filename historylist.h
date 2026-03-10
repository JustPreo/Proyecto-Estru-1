#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include "HistoryNode.h"

class HistoryList
{
private:

    HistoryNode* current;

public:

    HistoryList();
    void visit(Directory* dir);

    Directory* goBack();

    Directory* goForward();

    bool canGoBack();

    bool canGoForward();
};

#endif
