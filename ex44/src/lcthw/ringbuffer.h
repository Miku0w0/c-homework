#ifndef _lcthw_RingBuffer_h // 防止头文件被重复包含
#define _lcthw_RingBuffer_h

#include <lcthw/bstrlib.h> // 字符串处理功能的库

typedef struct
{
    char *buffer; // 指向实际存储数据的内存区域
    int length;   // 缓冲区总长度，用多出的1字节来区分满/空
    int start;    // 开始位置
    int end;      // 结束位置
} RingBuffer;

// 创建指定长度的环形缓冲区
RingBuffer *RingBuffer_create(int length);

// 销毁缓冲区，释放资源
void RingBuffer_destroy(RingBuffer *buffer);

// 从缓冲区读取数据到 target 数组
int RingBuffer_read(RingBuffer *buffer, char *target, int amount);

// 将数据写入缓冲区
int RingBuffer_write(RingBuffer *buffer, char *data, int length); 

// 读取指定数量的数据为一个bstring
bstring RingBuffer_gets(RingBuffer *buffer, int amount); 

// 计算缓冲区中已存储的数据量
#define RingBuffer_available_data(B) (((B)->end - (B)->start + (B)->length) % (B)->length)

// 计算缓冲区中剩余的空间量
#define RingBuffer_available_space(B) ((B)->length - 1 - RingBuffer_available_data(B))

// 检查缓冲区是否已满
#define RingBuffer_full(B) (RingBuffer_available_data(B) == (B)->length - 1)

// 检查缓冲区是否为空
#define RingBuffer_empty(B) (RingBuffer_available_data((B)) == 0)

// 便捷宏，用于将bstring类型的数据写入缓冲区
#define RingBuffer_puts(B, D) RingBuffer_write((B), bdata((D)), blength((D)))

// 便捷宏，获取缓冲区中的所有数据
#define RingBuffer_get_all(B) RingBuffer_gets((B), RingBuffer_available_data((B)))

// 当前读位置
#define RingBuffer_starts_at(B) ((B)->buffer + (B)->start)

// 当前写位置
#define RingBuffer_ends_at(B) ((B)->buffer + (B)->end)

// 在读取操作后更新start位置
#define RingBuffer_commit_read(B, A) ((B)->start = ((B)->start + (A)) % (B)->length)

// 在写入操作后更新end位置
#define RingBuffer_commit_write(B, A) ((B)->end = ((B)->end + (A)) % (B)->length)

#endif