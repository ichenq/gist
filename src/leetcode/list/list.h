#pragma once

#include <stddef.h>

// Definition for singly - linked list.
struct ListNode {
    int val = 0;
    ListNode *next = NULL;
    ListNode(int x = 0) : val(x), next(NULL) {}
};