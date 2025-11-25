#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
    char *name;
    int age;
    int height;
    int weight;
};

struct Person *Person_create(char *name, int age, int height, int weight)
{
    struct Person *who = malloc(sizeof(struct Person));
    assert(who != NULL);

    who->name = strdup(name);//复制
    who->age = age;
    who->height = height;
    who->weight = weight;

    return who;
}

void Person_print(struct Person *who)
{
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

void Person_destroy(struct Person *who)
{
    assert(who != NULL);

    free(who->name);
    free(who);
}

int main(int argc, char *argv[])
{
    // make two people structures
    struct Person *joe = Person_create(
            "Joe Alex", 32, 64, 140);

    struct Person *frank = Person_create(
            "Frank Blank", 20, 72, 180);

    // print them out and where they are in memory
    printf("Joe is at memory location %p:\n", joe);
    Person_print(joe);

    printf("Frank is at memory location %p:\n", frank);
    Person_print(frank);

    // make everyone age 20 years and print them again
    joe->age += 20;
    joe->height -= 2;
    joe->weight += 40;
    printf("--------after change--------\n");
    Person_print(joe);

    frank->age += 20;
    frank->weight += 20;
    printf("--------after change--------\n");
    Person_print(frank);

    // destroy them both so we clean up
    Person_destroy(joe);
    Person_destroy(frank);

    return 0;
}
/*
    Heap vs Stack 对比：

    1. 内存分配：
       - Heap（堆版本）：使用 malloc 分配结构体和 strdup 分配字符串，需要手动 free
       - Stack（栈版本）：结构体和字符串（用了strcpy）直接在栈上分配，函数结束后自动释放，不需要 free

    2. 访问方式：
       - Heap：使用指针访问成员，例如 joe->age
       - Stack：直接访问结构体成员，例如 joe.age

    3. 字符串处理：
       - Heap：name 是 char*，用 strdup 复制字符串到堆上
       - Stack：name 是 char 数组，可以直接 strcpy 赋值，不需要 strdup

    4. 内存管理：
       - Heap：如果不调用 Person_destroy，会造成内存泄露
       - Stack：结构体自动释放，不会泄露

    5. 函数传参：
       - Heap：通常传指针给函数（Person_print(joe)）
       - Stack：可以直接传结构体本身（Person_print(joe)），函数内部会复制一份结构体

    总结：
    Heap 版本更灵活，适合动态数量的对象，但需要手动管理内存
    Stack 版本简单、安全，但对象数量和生命周期在编译时就固定
    */