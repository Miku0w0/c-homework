# 编译器选项
CC = cc
CFLAGS = -Wall -g

# 所有源文件
SOURCES := $(wildcard ex*.c)

# 可执行文件的目录
BIN_DIR = bin

# 对应的可执行文件
TARGETS := $(addprefix $(BIN_DIR)/, $(SOURCES:.c=))

# 默认执行
all: $(TARGETS)

# 创建 bin 目录
$(shell mkdir -p $(BIN_DIR))

# 编译单个文件的规则（输出到 bin 目录）
$(BIN_DIR)/%: %.c
	$(CC) $(CFLAGS) $< -o $@

# 清理生成的可执行文件
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean