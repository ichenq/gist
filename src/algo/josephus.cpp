// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <iostream>
#include <assert.h>

using namespace std;

struct ListNode
{
    int         index = 0;
    ListNode*   next = nullptr;
};

// a josephus problem solution with circular list
int Josephus(const int M, const int N)
{
    assert(M > 0 && N > 0);
    if (N == 1)
    {
        return 1;
    }

    ListNode *circle = new ListNode[N];
    for (int i = 0; i < N-1; i++)
    {
        circle[i].index = i + 1;
        circle[i].next = circle + i + 1;
    }
    circle[N - 1].index = N;
    circle[N - 1].next = circle; // make circle

    int size = N;
    ListNode* start = circle;
    ListNode* prev = nullptr;
    while (size > 1)
    {
        for (int i = 0; i < M; i++)
        {
            prev = start;
            start = start->next;
        }
        cout << start->index << " exit" << endl;
        start = start->next;
        prev->next = start;
        size--;
    }
    int laststand = start->index;
    delete circle;
    return laststand;
}
