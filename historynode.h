#ifndef HISTORYNODE_H
#define HISTORYNODE_H

#include "Directory.h"

class HistoryNode
{
public:

    Directory* dir;

    HistoryNode* next;
    HistoryNode* prev;

    HistoryNode(Directory* d);
};

#endif
