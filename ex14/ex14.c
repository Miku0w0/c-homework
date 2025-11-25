#include <stdio.h>
#include <ctype.h>
#include <string.h>

// forward declarations,前向声明，后面调用函数，后面再写函数
void print_letters(char arg[],int length);

void print_arguments(int argc, char *argv[])
{
    int i = 0;
    for(i = 1; i < argc; i++) {
        int len = strlen(argv[i]);
        print_letters(argv[i], len);
    }
}

void print_letters(char arg[], int length)
{
    int i = 0;
    for(i = 0; i < length; i++) {
        char ch = arg[i];
        if(isalpha(ch) || isblank(ch)) {// isdigit(ch)
            printf("'%c' == %d ", ch, ch);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}