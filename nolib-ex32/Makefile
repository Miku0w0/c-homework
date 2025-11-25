# 编译器
CC = gcc

# 编译选项
CFLAGS = -g -Wall -Wextra

# 源文件
SRCS = list.c test.c

# 目标可执行文件
TARGET = test

# 默认目标
all: $(TARGET)

# 编译链接
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# 清理编译文件
clean:
	rm -f $(TARGET) *.o
