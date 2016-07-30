// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

struct ListNode {
  struct ListNode* next;
};

// reverse a single linked list
struct ListNode* ReverseSingleList(struct ListNode* node) {
    struct ListNode* pre = NULL;
    while (node)
    {
        struct ListNode* next = node->next;
        node->next = pre;
        pre = node;
        node = next;
    }
    return pre;
}
