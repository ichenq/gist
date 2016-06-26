
struct ListNode {
  struct ListNode* next;
};

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