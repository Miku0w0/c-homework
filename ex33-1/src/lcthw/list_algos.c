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
    if (!list || !list->first || !list->first->next) // 空链表
    {
        return NULL;
    }

    int cnt = List_count(list); // 链表的节点数
    int c1 = 1, c2 = 0; // c1为前半段，c2为后半段
    ListNode *slow = list->first;
    ListNode *fast = list->first->next;
    while (fast && fast->next) // 都不为空，直到快指针指向NULL
    {
        slow = slow->next; // 一步
        fast = fast->next->next; // 两步
        c1++;
    }
    c2 = cnt - c1;

    List *listsecond = List_create();
    listsecond->first = slow->next; // 后半部分的头
    listsecond->first->prev = NULL; // 前后链表断开
    listsecond->last = list->last;  // 后半部分的尾
    listsecond->count = c2;         // 后半部分的长度

    list->last = slow; // 前半部分的尾
    slow->next = NULL; // 前后链表断开
    list->count = c1;  // 前半部分的长度 

    return listsecond; // 返回给上一层递归
}

int List_insert_sorted(List *list, void *value, List_compare cmp)
{
    check(list != NULL, "List is NULL.");
    check(cmp != NULL, "Compare function is NULL.");

    ListNode *node = calloc(1, sizeof(ListNode)); // 开辟空间
    check_mem(node);

    node->value = value;

    // 空链表 或 插在头部
    if (list->first == NULL || cmp(value, list->first->value) < 0)
    {
        node->next = list->first;
        list->first = node;
        if (list->last == NULL) // 如果是新链表，要更新last
            list->last = node;
        list->count++;
        return 0;
    }

    // 查找插入位置
    ListNode *cur = list->first; // 找第一个比新值大的节点
    while (cur->next != NULL && cmp(value, cur->next->value) > 0)
    {
        cur = cur->next; // 第一个 比新值大的节点 之前的节点
    }

    node->next = cur->next; // 插在cur后面，cur->next才是第一个比新值大的节点
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
