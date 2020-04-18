#include <stdio.h>

/*
 * void* realloc(void* pv, size_t size);
 * 
 * 如果size小于原来pv内存块长度 截断到size大小 释放尾部内存并返回只有size长度的pv
 * 如果size大于原来pv内存块的长度 先分配申请一块size大小的内存 可能会被分配到别的地方 
 把原来pv内存块的内容复制到新内存块 但是后续扩大的内存块不会初始化 返回更大内存块的地址
 * 如果满足不了扩大内存的请求 即realloc调用失败 将pv置为NULL返回 如果原来的pv有保存数据 那这一块内存将丢失 造成内存泄漏
 * 当缩小内存大小时 realloc总是会调用成功
 * 如果pv为NULL 相当于调用 malloc(size); malloc(0)也合法
 * 如果pv不为NULL size为0时 相当于调用 free(pv);
 * 如果pv为NULL 且 size为0时 调用结果未定义
 */
