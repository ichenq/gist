#pragma once

#include <stddef.h>
#include <iostream>

// Definition for singly - linked list.
struct ListNode 
{
    int val = 0;
    ListNode *next = NULL;
    ListNode(int x = 0) : val(x), next(NULL) {}
};


static inline ListNode* arrayToList(int array[], int size)
{
    if (size == 0)
        return NULL;
    ListNode* head = NULL;
    ListNode* prev = NULL;
    for (int i = 0; i < size; i++)
    {
        ListNode* node = new ListNode(array[i]);
        if (head == NULL)
        {
            head = node;
            prev = node;
        }
        else
        {
            prev->next = node;
            prev = node;
        }
    }
    return head;
}

static inline int list_size(struct ListNode* node) {
    int size = 0;
    while (node)
    {
        node = node->next;
        size++;
    }
    return size;
}

static inline void delete_list(ListNode* node)
{
    while (node != NULL)
    {
        ListNode* next = node->next;
        delete node;
        node = next;
    }
}

static inline void print_list(ListNode* node)
{
    while (node != nullptr)
    {
        ListNode* next = node->next;
        std::cout << node->val;
        if (next != nullptr)
            std::cout << " -> ";
        node = next;
    }
    std::cout << std::endl;
}
