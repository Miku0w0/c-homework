#include <stdio.h>

//  函数版本
void print_using_pointers(char **names, int *ages, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s has %d years alive.\n", *(names+i), *(ages+i));
    }
    /*
    int i = 0;
    while(i < count){
        printf("%s has %d years alive.\n", *(names+i), *(ages+i));
        i ++;
    }*/
}

int main(int argc, char *argv[])
{
    /*for (char **arg = argv + 1; arg < argv + argc; arg++) {
        printf("Arg: %s\n", *arg); // + 1 argv本质指针作为函数参数的时候
    }*/
    // create two arrays we care about
    int ages[] = {23, 43, 12, 89, 2};
    char *names[] = {
        "Alan", "Frank",
        "Mary", "John", "Lisa"
    };

    // safely get the size of ages
    int count = sizeof(ages) / sizeof(int);
    int i = 0;

    // first way using indexing
    for(i = 0; i < count; i++) {
        printf("%s has %d years alive.\n",
                names[i], ages[i]);
    }

    /*for(i = 0;i < count;i ++){
        printf("%s has %d years alive.\n",*(names +i),*(ages + i));
    }*/

    /*while (i < count) {
        printf("%s has %d years alive.\n", names[i], ages[i]);
        i++;
    }*/
    printf("---\n");

    // setup the pointers to the start of the arrays
    int *cur_age = ages;
    char **cur_name = names;

    // second way using pointers
    for(i = 0; i < count; i++) {
        printf("%s is %d years old.\n",
                *(cur_name+i), *(cur_age+i));
    }

    /*for(i = 0;i < count;i ++){
        printf("%s has %d years alive.\n",cur_name[i],cur_age[i]);
    }*/

    /*while (i < count) {
        printf("%s is %d years old.\n", *(cur_name+i), *(cur_age+i));
        i++;
    }*/
    printf("---\n");

    // third way, pointers are just arrays
    for(i = 0; i < count; i++) {
        printf("%s is %d years old again.\n",
                cur_name[i], cur_age[i]);
    }

    /*while (i < count) {
        printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
        i++;
    }*/
    printf("---\n");

    // fourth way with pointers in a stupid complex way
    for(cur_name = names, cur_age = ages;
            (cur_age - ages) < count;
            cur_name++, cur_age++)
    {
        printf("%s lived %d years so far.\n",
                *cur_name, *cur_age);
    }
    /*cur_name = names;
    cur_age = ages;
    while ((cur_age - ages) < count) {
        printf("%s lived %d years so far.\n", *cur_name, *cur_age);
        cur_name++;
        cur_age++;
    }*/

    // 打印指针的地址
    printf("--- Pointer addresses ---\n");
    for (int i = 0; i < count; i++) {
        printf("names[%d] @ %p, ages[%d] @ %p\n",
            i, (void*)&names[i], i, (void*)&ages[i]);
    }
    /*while (i < count) {
        printf("names[%d] @ %p, ages[%d] @ %p\n",
            i, (void*)&names[i], i, (void*)&ages[i]);
        i++;
    }*/

    printf("--- Function version ---\n");
    print_using_pointers(names, ages, count);

    return 0;
}