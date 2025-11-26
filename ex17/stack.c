#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 10

typedef struct
{
    int items[MAX_SIZE]; // 存储元素
    int top;             // 栈顶索引
} Stack;

void initializeStack(Stack *s)
{ 
    s->top = -1;
}

int isEmpty(Stack *s)
{ 
    return s->top == -1;
}

int isFull(Stack *s)
{ 
    return s->top == MAX_SIZE - 1;
}

void push(Stack *s, int item)
{ 
    if (isFull(s))
    { 
        printf("Stack is full!\n");
    }
    else
    {
        s->top++;                // 先让top指向新元素的位置
        s->items[s->top] = item; // 再放入元素
    }
}

int pop(Stack *s)
{ 
    if (isEmpty(s))
    { 
        printf("Stack is empty!\n");
        return -1;
    }
    int value = s->items[s->top]; // 先取出栈顶元素
    s->top--;                     // 再让top指向下一个元素
    return value;
}

int peek(Stack *s)
{ 
    if (isEmpty(s))
    {
        printf("Stack is empty!\n");
        return -1;
    }
    else
    {
        return s->items[s->top];
    }
}

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