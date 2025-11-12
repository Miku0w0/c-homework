#include <stdio.h>
#include <string.h>

// 定义结构体
struct Person {
    char name[50]; // 栈上分配字符串，不用 strdup
    int age;
    int height;
    int weight;
};

// 打印函数，直接传结构体，不用指针
void Person_print(struct Person p) {
    printf("Name: %s\n", p.name);
    printf("\tAge: %d\n", p.age);
    printf("\tHeight: %d\n", p.height);
    printf("\tWeight: %d\n", p.weight);
}

int main(int argc, char *argv[])
{
    // 栈上创建结构体
    struct Person joe;
    struct Person frank;

    // 初始化结构体成员
    strcpy(joe.name, "Joe Alex"); // 栈上数组赋值
    joe.age = 32;
    joe.height = 64;
    joe.weight = 140;

    strcpy(frank.name, "Frank Blank");
    frank.age = 20;
    frank.height = 72;
    frank.weight = 180;

    // 打印结构体信息
    printf("Joe info:\n");
    Person_print(joe);

    printf("Frank info:\n");
    Person_print(frank);

    // 修改结构体成员
    joe.age += 20;
    joe.height -= 2;
    joe.weight += 40;

    frank.age += 20;
    frank.weight += 20;

    printf("--------after change--------\n");
    Person_print(joe);
    printf("--------after change--------\n");
    Person_print(frank);

    // 栈上分配，不用手动释放内存

    return 0;
}
