#include "darray.h"
#include "dbg.h"
#include <stdlib.h>
#include <string.h>

DArray *DArray_create(size_t element_size, size_t initial_max)
{
    DArray *array = malloc(sizeof(DArray));
    assert(array != NULL);

    array->contents = calloc(initial_max, sizeof(void *));
    assert(array->contents != NULL);

    array->end = 0;
    array->max = initial_max;
    array->element_size = element_size;

    return array;
}

void DArray_destroy(DArray *array)
{
    if (array)
    {
        free(array->contents);
        free(array);
    }
}

void DArray_clear(DArray *array)
{
    for (size_t i = 0; i < array->end; i++)
    {
        free(array->contents[i]);
        array->contents[i] = NULL;
    }
    array->end = 0;
}

void DArray_expand(DArray *array)
{
    array->max *= 2;
    array->contents = realloc(array->contents, array->max * sizeof(void *));
    assert(array->contents != NULL);
}

void DArray_push(DArray *array, void *el)
{
    if (array->end >= array->max)
    {
        DArray_expand(array);
    }
    array->contents[array->end++] = el;
}

void *DArray_pop(DArray *array)
{
    if (array->end == 0)
        return NULL;
    return array->contents[--array->end];
}

void *DArray_get(DArray *array, size_t i)
{
    if (i >= array->end)
        return NULL;
    return array->contents[i];
}

size_t DArray_count(DArray *array)
{
    return array->end;
}
