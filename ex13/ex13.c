#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("ERROR: You need at least one argument.\n");
        return 1;
    }

    // 外层循环：处理所有命令行参数
    for (int argn = 1; argn < argc; argn++) {
        printf("---- Argument %d: %s ----\n", argn, argv[argn]);

        // 内层循环：处理参数中的每个字母
        for (int i = 0, letter = argv[argn][i];
             letter != '\0';
             i++, letter = argv[argn][i]) {  // 用 ',' 初始化并更新 letter

            // 转为小写
            if (letter >= 'A' && letter <= 'Z') {
                letter += 32;
            }

            switch (letter) {
                case 'a':
                    printf("%d: 'A'\n", i);
                    break;

                case 'e':
                    printf("%d: 'E'\n", i);
                    break;

                case 'i':
                    printf("%d: 'I'\n", i);
                    break;

                case 'o':
                    printf("%d: 'O'\n", i);
                    break;

                case 'u':
                    printf("%d: 'U'\n", i);
                    break;

                case 'y':
                    if (i > 2) {
                        printf("%d: 'Y'\n", i);
                        //break; // break 在 if 内
                    }
                    break;
                    //把 break 放进 if 块中 
                    //当 i > 2：输出 'Y' 然后跳出
                    //当 i <= 2 时继续执行 default 分支，
                    //从而额外输出一条 “is not a vowel” 信息。
                    //原代码（break 在 if 外）才是预期行为。
                default:
                    printf("%d: %c is not a vowel\n", i, argv[argn][i]);
            }
        }
    }

    return 0;
}
