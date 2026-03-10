#include "historynode.h"

HistoryNode::HistoryNode(Directory* d) {
    dir = d;
    next = NULL;
    prev = NULL;
}
