#include "minunit.h"
#include "../src/lcthw/stack.h"
#include <assert.h>

static Stack stack; // 注意，不是指针
char *tests[] = {"test1 data", "test2 data", "test3 data"};
#define NUM_TESTS 3

char *test_create()
{
    stack = Stack_create();
    mu_assert(stack.array != NULL, "Failed to create stack.");
    return NULL;
}

char *test_push_pop()
{
    int i = 0;

    // 压栈
    for (i = 0; i < NUM_TESTS; i++)
    {
        Stack_push(stack, tests[i]);
        mu_assert(Stack_peek(stack) == tests[i], "Wrong next value.");
    }

    mu_assert(Stack_count(stack) == NUM_TESTS, "Wrong count on push.");

    // 遍历栈
    void *cur;
    STACK_FOREACH(stack, cur, idx)
    {
        debug("VAL[%zu]: %s", idx, (char *)cur);
    }

    // 弹栈
    for (i = NUM_TESTS - 1; i >= 0; i--)
    {
        char *val = Stack_pop(stack);
        mu_assert(val == tests[i], "Wrong value on pop.");
    }

    mu_assert(Stack_count(stack) == 0, "Wrong count after pop.");

    return NULL;
}

char *test_destroy()
{
    Stack_destroy(stack);
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_push_pop);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);