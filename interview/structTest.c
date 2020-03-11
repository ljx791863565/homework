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
struct st_task
{
	uint16_t id;
	uint32_t value;
	uint64_t timestamp;
};

int main()
{
	struct st_task task = {};
	uint64_t a = 0x1111123412345678;

	memcpy(&task, &a, sizeof(uint64_t));

	printf("%11u, %11u, %llu\n", task.id, task.value, task.timestamp);

	return 0;
}
