#include <iostream>
#include <vector>


struct ListNode {
    int val;
    ListNode *next;
    ListNode(int val): val(val) {}
};


/*********** merge sort ************/
ListNode* merge(ListNode* l, ListNode* r) {
    ListNode* result = new ListNode(0), *r_head = result;

    while (l != NULL && r != NULL) {
        if (l->val <= r->val) {
            r_head->next = l;
            r_head = r_head->next;
            l = l->next;
        }
        else {
            r_head->next = r;
            r_head = r_head->next;
            r = r->next;
        }
    }
    while (l != NULL) {
        r_head->next = l;
        r_head = r_head->next;
        l = l->next;
    }

    while (r != NULL) {
        r_head->next = r;
        r_head = r_head->next;
        r = r->next;
    }

    r_head = result->next;
    delete result;
    return r_head;
}
ListNode* merge_sort(ListNode* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    ListNode *q_head = head, *s_head = head, *pre = NULL;
    
    if (q_head != NULL && q_head->next != NULL) {
        q_head = q_head->next->next;
        pre = s_head;
        s_head = s_head->next;
    }
    // split the list to two parts
    pre->next = NULL;
    ListNode *l, *r;
    l = merge_sort(head);
    r = merge_sort(s_head);
    return merge(l, r);
}


void exchange_node(ListNode* pre1, ListNode* pre2) {
    if (pre1 == NULL || pre2 == NULL) {
        return;
    }
    ListNode *node_1 = pre1->next, *node_2 = pre2->next;
    if (node_1 == NULL || node_2 == NULL || node_1 == node_2) {
        return;
    }

    if (node_1 == pre2) {
        // pre1 -> pre2 -> node_2
        // exchange pre2 and node_2
        pre1->next = node_2;
        node_1->next = node_2->next;
        node_2->next = node_1;
    }
    else if (node_2 == pre1) {
        pre2->next = node_1;
        node_2->next = node_1->next;
        node_1->next = node_2;
    }
    else {
        pre1->next = node_2;
        pre2->next = node_1;
        ListNode* tmp;
        tmp = node_2->next;
        node_2->next = node_1->next;
        node_1->next = tmp;
    }
}
ListNode* partition(ListNode* head, ListNode* start, ListNode** end) {
    ListNode *x = start, *y = start->next, *y_pre = start;
    // x->next always point to the first one > end
    while (y && y != *end) {
        if (y->val < (*end)->val) {
            exchange_node(x, y_pre);
            x = x->next;
        }
        y_pre = y;
        y = y->next;
    }

    exchange_node(x, y_pre);
    *end = y_pre->next;
    return x;
}
void quick_sort(ListNode *head, ListNode *start, ListNode *end) {
    if (start == NULL || end == NULL || start == end || start->next == end) {
        return;
    }
    ListNode *tmp = partition(head, start, &end);
    quick_sort(head, start, tmp);
    quick_sort(head, tmp->next, end);
}