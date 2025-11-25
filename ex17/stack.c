#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 10

typedef struct
{
    int items[MAX_SIZE]; // 存储元素
    int top; // 栈顶索引
} Stack;

// 栈的初始化
void initializeStack(Stack *s)
{
    s->top = -1; // 标记为空
}

// 检查栈是否为空
int isEmpty(Stack *s)
{
    return s->top == -1; // 标记为空
}

// 检查栈是否已满
int isFull(Stack *s)
{
    return s->top == MAX_SIZE - 1; // 标记为满
}

// 入栈
void push(Stack *s, int item)
{
    if (isFull(s))
    {
        printf("Stack is full!\n");
    }
    else
    {
        s->top ++;
        s->items[s->top] = item;
    }
}

// 出栈
int pop(Stack *s)
{
    if (isEmpty(s))
    {
        printf("Stack is empty!\n");
        return -1; // 表示栈空
    }

        int value = s->items[s->top];
        s->top --;
        return value;
    
}

// 查看栈顶元素
int peek(Stack *s)
{
    if (isEmpty(s))
    {
        printf("Stack is empty!\n");
        return -1; // 返回一个标识值，表示栈空
    }
    else
    {
        return s->items[s->top];
    }
}

// 主函数，演示栈操作
int main()
{
    Stack s;
    initializeStack(&s);

    // 入栈操作
    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    // 查看栈顶元素
    printf("Top element is %d\n", peek(&s));

    // 出栈操作，并打印出栈元素
    printf("Popped %d from the stack\n", pop(&s));
    printf("Popped %d from the stack\n", pop(&s));

    // 再次查看栈顶元素
    if (!isEmpty(&s))
    {
        printf("Top element is %d\n", peek(&s));
    }

    // 清空栈
    while (!isEmpty(&s))
    {
        pop(&s);
    }

    if (isEmpty(&s))
    {
        printf("Stack is empty now.\n");
    }

    return 0;
}