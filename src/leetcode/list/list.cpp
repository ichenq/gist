
#include "list.h"
#include <iostream>
#include <vector>

using namespace std;

static ListNode* arrayToList(int array[], int size)
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

static void delete_list(ListNode* node)
{
    while (node != NULL)
    {
        ListNode* next = node->next;
        delete node;
        node = next;
    }
}

static void print_list(ListNode* node)
{
    while (node != nullptr)
    {
        ListNode* next = node->next;
        cout << node->val;
        if (next != nullptr)
            cout << " -> ";
        node = next;
    }
    cout << endl;
}


// convert binary(in list node) to integer
// https://leetcode.com/problems/convert-binary-number-in-a-linked-list-to-integer/
int getDecimalValue(struct ListNode* head) {
    int num = 0;
    struct ListNode* node = head;
    while (node != NULL) {
        num = num << 1;     // shift left
        num |= node->val;    // bit or
        node = node->next;
    }
    return num;
}

// get middle node of a list
// https://leetcode.com/problems/middle-of-the-linked-list/
struct ListNode* middleNode(struct ListNode* head) {
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// reverse a single linked list
// https://leetcode.com/problems/reverse-linked-list/
ListNode* reverseList(ListNode* node) {
    ListNode* prev = NULL;
    while (node)
    {
        ListNode* next = node->next;
        node->next = prev;
        prev = node;
        node = next;
    }
    return prev;
}

// merge 2 sorted linked list
// https://leetcode.com/problems/merge-two-sorted-lists/
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy = {};
    ListNode* p = &dummy;
    while (l1 != NULL && l2 != NULL)
    {
        if (l1->val < l2->val) {
            p->next = l1;
            l1 = l1->next;
        }
        else {
            p->next = l2;
            l2 = l2->next;
        }
        p = p->next;
    }
    p->next = (l1 == NULL) ? l2 : l1;
    return dummy.next;
}

void test_list()
{
    int arr1[] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 };
    int arr3[] = {2, 4, 3};
    int arr2[] = {5, 6, 4};
    int arr4[] = {1, 2, 3, 4, 5, 6, 7};
    int arr5[] = { 1 };
    int arr6[] = { 1, 3, 5, 7, 9 };
    int arr7[] = { 2, 4, 6, 8, 10 };
    int arr8[] = { 1, 1, 0, 1, 0, 1 };
    {
        ListNode* list1 = arrayToList(arr8, _countof(arr8));
        int n = getDecimalValue(list1);
        cout << n << endl;
    }
    {
        ListNode* list1 = arrayToList(arr1, _countof(arr1));
        ListNode* list = deleteDuplicates(list1);
        print_list(list);
    }
    {
        ListNode* list1 = arrayToList(arr6, _countof(arr6));
        ListNode* list2 = arrayToList(arr7, _countof(arr7));
        ListNode* list = mergeTwoLists(list1, list2);
        print_list(list);
        delete(list1);
    }
    {
        ListNode* list1 = arrayToList(arr5, _countof(arr5));
        // ListNode* list = removeNthFromEnd(list1, 7);
        ListNode* list = removeNthFromEnd2(list1, 1);
        print_list(list);
        delete_list(list1);
    }
    {
        ListNode* list1 = arrayToList(arr1, _countof(arr1));
        ListNode* list2 = arrayToList(arr2, _countof(arr2));
        ListNode* list3 = addTwoNumbers(list1, list2);
        print_list(list3);
        delete_list(list1);
        delete_list(list2);
    }

}
