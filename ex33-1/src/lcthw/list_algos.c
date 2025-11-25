#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

inline void ListNode_swap(ListNode *a, ListNode *b)
{
    void *temp = a->value;
    a->value = b->value;
    b->value = temp;
}

int List_bubble_sort(List *list, List_compare cmp)
{
    int sorted = 1;

    if (List_count(list) <= 1)
    {
        return 0; // already sorted
    }

    do
    {
        sorted = 1;
        LIST_FOREACH(list, first, next, cur)
        {
            if (cur->next)
            {
                if (cmp(cur->value, cur->next->value) > 0)
                {
                    ListNode_swap(cur, cur->next);
                    sorted = 0;
                }
            }
        }
    } while (!sorted);

    return 0;
}
List *List_merge(List *left, List *right, List_compare cmp)
{
    List *result = List_create();

    while (left->first && right->first)
    {
        if (cmp(left->first->value, right->first->value) <= 0)
        {
            List_push(result, List_shift(left));
        }
        else
        {
            List_push(result, List_shift(right));
        }
    }
    while (left->first)
    {
        List_push(result, List_shift(left));
    }
    while (right->first)
    {
        List_push(result, List_shift(right));
    }

    return result;
}

List *List_merge_sort(List *list, List_compare cmp)
{
    if (List_count(list) <= 1)
    {
        return list;
    }

    List *secondHalf = List_split_mid(list);

    List *sort_left = List_merge_sort(list, cmp);
    List *sort_right = List_merge_sort(secondHalf, cmp);

    if (sort_left != list)
        List_destroy(list);
    if (sort_right != secondHalf)
        List_destroy(secondHalf);

    return List_merge(sort_left, sort_right, cmp);
}

List *List_split_mid(List *list)
{
    if (!list || !list->first || !list->first->next)
    {
        return NULL;
    }

    int cnt = List_count(list);
    int c1 = 1, c2 = 0;
    ListNode *slow = list->first;
    ListNode *fast = list->first->next;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        c1++;
    }
    c2 = cnt - c1;

    List *listsecond = List_create();
    listsecond->first = slow->next;
    listsecond->first->prev = NULL;
    listsecond->last = list->last;
    listsecond->count = c2;
    list->last = slow;
    slow->next = NULL;
    list->count = c1;

    return listsecond;
}

int List_insert_sorted(List *list, void *value, List_compare cmp)
{
    check(list != NULL, "List is NULL.");
    check(cmp != NULL, "Compare function is NULL.");

    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    // 空链表 或 插在头部
    if (list->first == NULL || cmp(value, list->first->value) < 0)
    {
        node->next = list->first;
        list->first = node;
        if (list->last == NULL)
            list->last = node;
        list->count++;
        return 0;
    }

    // 查找插入位置
    ListNode *cur = list->first;
    while (cur->next != NULL && cmp(value, cur->next->value) > 0)
    {
        cur = cur->next;
    }

    node->next = cur->next;
    cur->next = node;

    // 如果插在尾部
    if (node->next == NULL)
    {
        list->last = node;
    }

    list->count++;
    return 0;

error:
    return -1;
}
