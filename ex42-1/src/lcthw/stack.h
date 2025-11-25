#ifndef STACK_DARRAY_H
#define STACK_DARRAY_H

#include "darray.h"

typedef struct Stack
{
    DArray *array;
} Stack;

#define Stack_create() \
    (Stack) { DArray_create(sizeof(void *), 10) }
#define Stack_destroy(S) DArray_destroy((S).array)
#define Stack_push(S, V) DArray_push((S).array, V)
#define Stack_pop(S) DArray_pop((S).array)
#define Stack_peek(S) ((S).array->end ? (S).array->contents[(S).array->end - 1] : NULL)
#define Stack_count(S) DArray_count((S).array)

// 遍历栈，从栈顶到底
#define STACK_FOREACH(S, V, I) \
    for (size_t I = (S).array->end; I > 0 && ((V) = (S).array->contents[I - 1], 1); I--)

#endif
