#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <time.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
#define NUM_VALUES 5

List *create_words()
{
    List *words = List_create();

    for (int i = 0; i < NUM_VALUES; i++)
    {
        List_push(words, values[i]);
    }

    return words;
}

int is_sorted(List *words)
{
    LIST_FOREACH(words, first, next, cur)
    {
        if (cur->next && strcmp(cur->value, cur->next->value) > 0)
        {
            debug("%s %s", (char *)cur->value, (char *)cur->next->value);
            return 0;
        }
    }

    return 1;
}

char *test_bubble_sort()
{
    List *words = create_words();

    // should work on a list that needs sorting
    int rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort failed.");
    mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

    // should work on an already sorted list
    rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort of already sorted failed.");
    mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");

    List_destroy(words);

    // should work on an empty list
    words = List_create(words);
    rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort failed on empty list.");
    mu_assert(is_sorted(words), "Words should be sorted if empty.");

    List_destroy(words);

    return NULL;
}

char *test_merge_sort()
{
    List *words = create_words();

    // should work on a list that needs sorting
    List *res = List_merge_sort(words, (List_compare)strcmp);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

    List *res2 = List_merge_sort(res, (List_compare)strcmp);
    mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
    List_destroy(res2);
    List_destroy(res);

    List_destroy(words);
    return NULL;
}

char *test_insert_sorted()
{
    List *list = List_create();

    List_insert_sorted(list, "delta", (List_compare)strcmp);
    List_insert_sorted(list, "alpha", (List_compare)strcmp);
    List_insert_sorted(list, "charlie", (List_compare)strcmp);
    List_insert_sorted(list, "bravo", (List_compare)strcmp);

    mu_assert(strcmp(list->first->value, "alpha") == 0, "alpha should be first");
    mu_assert(strcmp(list->last->value, "delta") == 0, "delta should be last");

    ListNode *n = list->first;
    mu_assert(strcmp(n->value, "alpha") == 0, "1");
    n = n->next;
    mu_assert(strcmp(n->value, "bravo") == 0, "2");
    n = n->next;
    mu_assert(strcmp(n->value, "charlie") == 0, "3");
    n = n->next;
    mu_assert(strcmp(n->value, "delta") == 0, "4");

    List_destroy(list);
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    double start = clock();
    mu_run_test(test_bubble_sort);
    double end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;// 周期数转为秒
    printf("test_bubble_sort took %f seconds to execute \n", cpu_time_used);

    start = clock();
    mu_run_test(test_merge_sort);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("test_merge_sort took %f seconds to execute \n", cpu_time_used);

    start = clock();
    mu_run_test(test_insert_sorted);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("test_insert_sorted took %f seconds\n", cpu_time_used);

    return NULL;
}

RUN_TESTS(all_tests);