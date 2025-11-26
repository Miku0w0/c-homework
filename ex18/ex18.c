#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* 错误处理函数 */
void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1); // 非正常退出
}

/* typedef 函数指针 */
/* 指向 带有两个 int 参数并返回 int 的函数 的指针类型 */
typedef int (*compare_cb)(int a, int b);

/* 冒泡排序函数 */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
    int temp = 0;
    int i = 0;
    int j = 0;

    int *target = malloc(count * sizeof(int)); // 分配空间
    if(!target) die("Memory error.");

    memcpy(target, numbers, count * sizeof(int)); // 把原数组复制到target数组

    for(i = 0; i < count; i++) {
        for(j = 0; j < count - 1; j++) {
            if(cmp(target[j], target[j+1]) > 0) { // 左 > 右
                temp = target[j+1];
                target[j+1] = target[j];
                target[j] = temp;
            }
        }
    }

    return target; // 排序后的数组指针
}

// 新增插入排序函数
int *insertion_sort(int *numbers, int count, compare_cb cmp)
{
    int *target = malloc(count * sizeof(int)); // 分配空间
    if(!target) die("Memory error.");

    memcpy(target, numbers, count * sizeof(int)); // 把原数组复制到target数组

    for(int i = 1; i < count; i++) {
        int key = target[i]; // 待插入的元素，从第二个开始
        int j = i - 1;       // 从右向左寻找合适的位置
        while(j >= 0 && cmp(target[j], key) > 0) { // 判断前面的元素是否比key大
            target[j+1] = target[j]; // 前面元素有比key大的，就往后挪
            j--;                     // 向左继续比较
        }
        target[j+1] = key; // 插入
    }
    return target;
}
/* 插入排序的例子
i=4, key=1
j=3, cmp(6,1)>0 → move 6
target = [2, 4, 5, 6, 6]
j=2, cmp(5,1)>0 → move 5
target = [2, 4, 5, 5, 6]
j=1, cmp(4,1)>0 → move 4
target = [2, 4, 4, 5, 6]
j=0, cmp(2,1)>0 → move 2
target = [2, 2, 4, 5, 6]
j=-1 → exit while
Insert key at j+1=0
target = [1, 2, 4, 5, 6]
*/

int sorted_order(int a, int b) // 升序
{
    return a - b; 
}

int reverse_order(int a, int b) // 降序
{
    return b - a; 
}

int strange_order(int a, int b) // 奇怪顺序
{
    if(a == 0 || b == 0) {
        return 0;
    } else {
        return a % b;
    }
}

void wrong_func(int a, int b) { // 错误函数，参数或返回类型不对
    printf("I am wrong\n");
}

/**
 * 待排序原数组numbers，数组长度count，排序函数指针，比较函数指针
 * (*sort_algo)表示sort_algo是一个指针，
 * 返回类型是 int*数组，参数类型是 (int*, int, compare_cb)，
 * 分别是 一个数组指针，一个数组长度，一个比较函数指针
 */
void test_sorting(int *numbers, int count,int* (*sort_algo)(int*, int, compare_cb), compare_cb cmp)
{
    int i = 0;
    int *sorted = sort_algo(numbers, count, cmp); // 排序

    if(!sorted) die("Failed to sort as requested.");

    for(i = 0; i < count; i++) { // 打印排序后的数组
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(sorted); // 释放内存
}


int main(int argc, char *argv[])
{
    if(argc < 2) die("USAGE: ex18 4 3 1 5 6");

    int count = argc - 1; // 数字个数
    int i = 0; // 循环变量
    char **inputs = argv + 1; // 指向 命令行数字参数 的指针数组，方便转int

    int *numbers = malloc(count * sizeof(int)); // 分配空间，接收命令行参数转换结果
    if(!numbers) die("Memory error.");

    for(i = 0; i < count; i++) { // 字符串转为整数，变为排序函数里面的待排序数组
        numbers[i] = atoi(inputs[i]);
    }

    // 冒泡排序
    printf("Bubble sort sorted/reverse/strange order:\n");
    test_sorting(numbers, count, bubble_sort,sorted_order);

    test_sorting(numbers, count, bubble_sort,reverse_order);

    test_sorting(numbers, count, bubble_sort,strange_order);

    // 插入排序
    printf("Insertion sort sorted/reverse/strange order:\n");
    test_sorting(numbers, count, insertion_sort, sorted_order);

    test_sorting(numbers, count, insertion_sort, reverse_order);

    test_sorting(numbers, count, insertion_sort, strange_order);

    free(numbers);

    return 0;
}