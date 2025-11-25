#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>
#include <assert.h>

typedef struct DArray
{
    void **contents;
    size_t end; // 下一个插入的位置，也就是元素数量
    size_t max; // 数组容量
    size_t element_size;
} DArray;

DArray *DArray_create(size_t element_size, size_t initial_max);
void DArray_destroy(DArray *array);
void DArray_clear(DArray *array);
void DArray_expand(DArray *array);
void DArray_push(DArray *array, void *el);
void *DArray_pop(DArray *array);
void *DArray_get(DArray *array, size_t i);
size_t DArray_count(DArray *array);

#endif
