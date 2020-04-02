#include <stdio.h>
#include <inttypes.h>
/*
** typedef unsigned char int8_t;
** typedef unsigned int int16_t;
** typedef unsigned long int32_t;
** typedef unsigned long long int64_t;
*/


/*
 * 小端下的a内存排列为 假设地址从0开始
 *
 * 0x0		78
 * 0x7		56
 * 0x15		34
 * 0x23		12
 * 0x31		34
 * 0x39		12
 * 0x47		11
 * 0x55		11
 *  
 * 共占64bits 8bytes
 *
 * 结构体内存分布	|
 * -----------------------------------------------------------------
 * |		|///////|				|								|
 * -----------------------------------------------------------------
 * |id		|补位	|	value		|		timestamp				|
 *
 * 所以对应的内存拷贝情况就会是
 * -----------------------------------------------------------------
 * |7856	|3412	|32121111		|
 *
 */
#include <string.h>
/*
 * 对齐
 * 结构体中以最大长度数据类型的整数倍对齐内存 如果不够就补全到整数倍·
 */
struct st_task
{
	uint16_t id;
	uint32_t value;
	uint64_t timestamp;
};
struct st{
	int i;
	short b;
	char c;
};
struct st_c {
	short a;
	short b;
	short c;
};


//位域出现的原因是由于某些信息的存储表示只需要几个bit位就可以表示而不需要一个完整的字节，同时也是为了节省存储空间和方便处理。
struct A
{
	unsigned a:4;
	unsigned :2;	//无名位域 表示占位和调整位域 这个位域空间不可以被使用 也无法调用
	unsigned b:2;
	unsigned :0;	//空域 下一个位域要从下一个字节开始存放
	unsigned c:4;
	unsigned d:22;
};
int main()
{
	struct st_task task = {};
	uint64_t a = 0x1111123412345678;

	memcpy(&task, &a, sizeof(uint64_t));

	printf("%1u, %1u, %lu\n", task.id, task.value, task.timestamp);
	printf("%lu\n", sizeof(struct st));
	printf("%lu\n", sizeof(struct A));
	printf("%lu\n", sizeof(struct st_c));
	return 0;
}
