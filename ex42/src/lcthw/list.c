#include "list.h"
#include "dbg.h"

List *List_create()
{
    return calloc(1, sizeof(List));
}

void List_destroy(List *list)
{
    LIST_FOREACH(list, first, next, cur)
    {
        if (cur->prev)
        {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}

void List_clear(List *list)
{
    LIST_FOREACH(list, first, next, cur)
    {
        free(cur->value);
    }
}

/**
 * void List_clear_destroy(List *list)
{
    List_clear(list);
    List_destroy(list);
}
 */

void List_clear_destroy(List *list)
{
    check(list, "List is NULL.");
    LIST_FOREACH(list, first, next, cur)
    {
        // printf("freeing: %s\n", (char *)cur->value);
        free(cur->value);
        if (cur->prev)
        {
            free(cur->prev);
        }
    }
    free(list->last);
    free(list);
error:
    return;
}

void List_push(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->last == NULL)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

error:
    return;
}

void *List_pop(List *list)
{
    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->first == NULL)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

error:
    return;
}

void *List_shift(List *list)
{
    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node)
{
    void *result = NULL;

    check(list->first && list->last, "List is empty.");
    check(node, "node can't be NULL");

    if (node == list->first && node == list->last)
    {
        list->first = NULL;
        list->last = NULL;
    }
    else if (node == list->first)
    {
        list->first = node->next;
        check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
        list->first->prev = NULL;
    }
    else if (node == list->last)
    {
        list->last = node->prev;
        check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
        list->last->next = NULL;
    }
    else
    {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);

error:
    return result;
}

List *List_copy(List *list)
{
    check(list, "List is NULL.");

    List *new_list = List_create();
    check_mem(new_list);

    LIST_FOREACH(list, first, next, cur)
    {
        if (cur->value)
        {
            // 复制字符串。注意：strdup在内存分配失败时返回NULL
            char *copy_of_value = strdup((char *)cur->value);
            if (copy_of_value == NULL)
            {
                // 处理内存分配失败的情况
                List_destroy(new_list);
                return NULL;
            }
            List_push(new_list, copy_of_value);
        }
        else
        {
            // 如果当前节点的value为NULL，也可以选择将NULL推入新列表
            List_push(new_list, NULL);
        }
    }

    return new_list;

error:
    return NULL;
}

void List_join(List *first, List *second)
{
    check(first && second, "One of the lists is NULL.");

    if (second->first == NULL)
    {
        return; // 第二个链表为空，无需操作。
    }

    if (first->last != NULL)
    {
        first->last->next = second->first; // 连接两个链表。
        second->first->prev = first->last;
    }
    else
    {
        first->first = second->first; // 如果第一个链表为空，直接设置首尾指针。
    }

    first->last = second->last;
    first->count += second->count;

error:
    return;
}

List *List_split(List *list, ListNode *node)
{
    check(list && node, "List or node is NULL.");

    List *new_list = List_create();
    check_mem(new_list);

    ListNode *cur = node;
    while (cur != NULL)
    {
        ListNode *next = cur->next;
        List_push(new_list, cur->value);

        if (cur == list->last)
        {
            list->last = cur->prev;
        }

        if (cur->prev)
        {
            cur->prev->next = cur->next;
        }

        if (cur->next)
        {
            cur->next->prev = cur->prev;
        }

        free(cur);
        cur = next;
        list->count--;
    }

    if (list->last)
    {
        list->last->next = NULL;
    }

    if (list->count == 0)
    {
        list->first = NULL;
    }

    return new_list;

error:
    return NULL;
}