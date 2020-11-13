
#include "list.h"
#include <iostream>
#include <vector>

using namespace std;


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
ListNode* mergeTwoLists(ListNode* left, ListNode* right) {
    ListNode dummy = {};
    ListNode* p = &dummy;
    while (left != NULL && right != NULL)
    {
        if (left->val < right->val) {
            p->next = left;
            left = left->next;
        }
        else {
            p->next = right;
            right = right->next;
        }
        p = p->next;
    }
    p->next = (left == NULL) ? right : left;
    return dummy.next;
}

// Linked List Cycle
// https://leetcode.com/problems/linked-list-cycle/
bool hasCycle(struct ListNode* head) {
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == slow) {
            return true;
        }
    }
    return false;
}

// Remove Nth Node From End of List
// https://leetcode.com/problems/remove-nth-node-from-end-of-list/description/
ListNode* removeNthFromEnd(struct ListNode* head, int n)
{
    ListNode dummy;
    dummy.next = head;
    ListNode* second = &dummy;
    for (int i = 0; i <= n; i++) // move N+1 step
    {
        second = second->next;
    }
    ListNode* first = &dummy;
    while (second != NULL)
    {
        first = first->next;
        second = second->next;
    }
    first->next = first->next->next;
    return dummy.next;
}


// Add two non-negative integers represented by linked list in reverse order
// https://leetcode.com/problems/add-two-numbers/
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {

    int count1 = list_size(l1);
    int count2 = list_size(l2);

    ListNode* head = count1 > count2 ? l1 : l2;
    ListNode* other = head == l1 ? l2 : l1;
    ListNode* node = head;
    ListNode* prev = NULL;

    int carry = 0;
    while (node != NULL)
    {
        if (other != NULL)
        {
            node->val += other->val;
            other = other->next;
        }
        if (carry) {
            carry = 0;
            node->val++;
        }
        if (node->val >= 10) {
            carry = 1;
            node->val %= 10;
        }
        prev = node;
        node = node->next;
    }
    if (carry)
    {
        prev->next = new ListNode(1);
    }
    return head;
}

// remove duplicated from sorted list
// https://leetcode.com/problems/remove-duplicates-from-sorted-list/
struct ListNode* deleteDuplicates(struct ListNode* head)
{
    if (head == NULL || head->next == NULL)
        return head;
    struct ListNode* prev = head;
    struct ListNode* node = head->next;
    while (node != NULL)
    {
        if (node->val == prev->val)
        {
            prev->next = node->next; // remove node
        }
        else
        {
            prev = node;
        }
        node = node->next;
    }
    return head;
}

void test_list()
{
    int arr1[] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 };
    int arr3[] = { 2, 4, 3 };
    int arr2[] = { 5, 6, 4 };
    int arr4[] = { 1, 2, 3, 4, 5, 6, 7 };
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
        ListNode* list = removeNthFromEnd(list1, 1);
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
