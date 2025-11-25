#include <stdio.h>
#include <string.h>
#include "list.h"

int main()
{
    List *list = List_create();

    List_push(list, strdup("hello"));
    List_push(list, strdup("world"));

    printf("First: %s\n", (char *)List_first(list));
    printf("Last: %s\n", (char *)List_last(list));

    List_clear_destroy(list);
    return 0;
}
