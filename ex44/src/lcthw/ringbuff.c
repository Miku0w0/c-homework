#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lcthw/dbg.h>
#include <lcthw/ringbuffer.h>

RingBuffer *RingBuffer_create(int length)
{
    RingBuffer *buffer = calloc(1, sizeof(RingBuffer));
    buffer->length = length + 1; // 浪费一个字节判断是否为空
    buffer->start = 0;
    buffer->end = 0;
    buffer->buffer = calloc(buffer->length, 1);

    return buffer;
}

void RingBuffer_destroy(RingBuffer *buffer)
{
    if (buffer)
    {
        free(buffer->buffer);
        free(buffer);
    }
}

/**
 * 从一个指定源写入到环形缓冲区中
 * 错误检查、内存复制、更新缓冲区指针
 * buffer为目标环形缓冲区，data为写入缓冲区数据的指针，length为写入数据的长度
 */
int RingBuffer_write(RingBuffer *buffer, char *data, int length)
{
    // 确保写入数据的长度不超过缓冲区可用空间
    check(length <= RingBuffer_available_space(buffer),
          "Not enough space: %d request, %d available",
          RingBuffer_available_data(buffer), length);

    // memcpy将数据从data指针复制到缓冲区RingBuffer_ends_at(buffer)，并确保写入
    int end = buffer->end;
    int first = buffer->length - end;

    if (length <= first)
    {
        memcpy(buffer->buffer + end, data, length);
    }
    else
    {
        memcpy(buffer->buffer + end, data, first);
        memcpy(buffer->buffer, data + first, length - first);
    }

    // 更新end指针，成功则返回写入字节数
    RingBuffer_commit_write(buffer, length);
    return length;

error:
    return -1;
}

/**
 * 将数据从环形缓冲区复制到另一个缓冲区
 * buffer为指向RingBuffer结构的指针，target为目标缓冲区，amount为要从环形缓冲区读取量
 */
int RingBuffer_read(RingBuffer *buffer, char *target, int amount)
{
    check_debug(amount <= RingBuffer_available_data(buffer),
                "Not enough in the buffer: has %d, needs %d",
                RingBuffer_available_data(buffer), amount);

    // memcpy 从RingBuffer_starts_at(buffer)开始，复制amount字节到target
    int start = buffer->start;
    int first = buffer->length - start;

    if (amount <= first)
    {
        memcpy(target, buffer->buffer + start, amount);
    }
    else
    {
        memcpy(target, buffer->buffer + start, first);
        memcpy(target + first, buffer->buffer, amount - first);
    }

    RingBuffer_commit_read(buffer, amount);
    return amount;
error:
    return -1;
}

/**
 * 从环形缓冲区获取一定数量的数据，返回一个bstring对象
 * buffer为指向RingBuffer结构的指针，amount为要从环形缓冲区读取的数据量
 */
bstring RingBuffer_gets(RingBuffer *buffer, int amount)
{
    check(amount > 0, "Need more than 0 for gets, you gave: %d ", amount);
    check_debug(amount <= RingBuffer_available_data(buffer),
                "Not enough in the buffer.");

    // 用blk2bstr从环形缓冲区的start创建一个新的bstring对象
    bstring result = blk2bstr(RingBuffer_starts_at(buffer), amount);
    check(result != NULL, "Failed to create gets result.");
    check(blength(result) == amount, "Wrong result length.");

    // 读取操作以后更新 start 指针，检查长度是否正确
    RingBuffer_commit_read(buffer, amount);
    assert(RingBuffer_available_data(buffer) >= 0 && "Error in read commit.");

    // 返回一个bstring对象
    return result;
error:
    return NULL;
}