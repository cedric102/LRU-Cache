#pragma once

#include <memory>

// It corresponds to a pair in a dictionary.
// The LFU data member is necessary to keep track of how often a node has been accessed
// The LFU should be incremented upon each access
struct Node {
    shared_ptr<Node> prev = nullptr;
    shared_ptr<Node> next = nullptr;
    int64_t key = -1;
    int64_t value = -1;
    int64_t lfu = 0;
};
