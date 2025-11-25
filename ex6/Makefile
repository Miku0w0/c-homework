# 编译器选项
CC = cc
CFLAGS = -Wall -g

# 所有源文件
SOURCES := $(wildcard ex*.c)

# 对应的可执行文件
TARGETS := $(SOURCES:.c=)

# 默认执行
all: $(TARGETS)

# 编译单个文件的规则（输出到 bin 目录）
%: %.c
	$(CC) $(CFLAGS) $< -o $@

# 清理生成的可执行文件
clean:
	rm -rf $(TARGETS)

.PHONY: all clean