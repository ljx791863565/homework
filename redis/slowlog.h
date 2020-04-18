#ifndef _SLOW_LOG_H_
#define _SLOW_LOG_H_

#define SLOWLOG_ENTRY_MAX_ARGC 32
#define SLOWLOG_ENTRY_MAX_STRING 128

/*
 * slowlog 用于记录最新N条执行时间超过M ms的命令
 * 用于观察系统性能 
 * redisServer中用链表保存了slowlog 每个节点即一个slowlogEntry 
 * 
 * struct redisServer {
 *		// ... other fields
 *	
 *		// 保存慢查询日志的链表
 *		list *slowlog; 
 *		
 *		// 慢查询日志的当前id 值
 *		long long slowlog_entry_id; 
 *      
 *      // 慢查询时间限制
 *		long long slowlog_log_slower_than; 
 *		
 *		// 慢查询日志的最大条目数量
 *		unsigned long slowlog_max_len; 
 *		
 *		// ... other fields
 *  };
 *
 * 日志链表按照添加时间新旧排序 新的在链表左边 旧的日志记录在链表右边
 * slowlog_entry_id在创建每一条新的日志时加1 用于产生查询日志的ID
 * slowlog_log_slower_than 是用户指定的命令执行时间上限，执行时间大于等于这个值的命令会被慢查询日志记录
 * slowlog_max_len 慢查询日志的最大数量，当日志数量等于这个值时，添加一条新日志会造成最旧的一条日志被删除
 *
 * 在每次执行命令之前 Redis 都会用一个参数记录命令执行前的时间 在命令执行完之后 再计算一次当前时间 
 * 然后将两个时间值相减 得出执行命令所耗费的时间值duration  并将duration 传给 slowlogPushEntryIfNeeded 函数
 * 如果duration 超过服务器设置的执行时间上限 server.slowlog_log_slower_than 的话 
 * slowlogPushEntryIfNeeded 就会创建一条新的慢查询日志 并将它加入到慢查询日志链表里
 *
 * 三种操作
 *	查看日志：在日志链表中遍历指定日志节点
 *	清空日志：释放日志链表所有节点
 *	获取日志数量：获取日志链表中节点数量
 *
 *	慢查询日志保存在内存而不是文件中 确保日志不会成为性能瓶颈
 */

typedef struct slowlogEntry {
	//命令参数
	robj **argv;
	//命令参数的数量
	robj argc;

	long long id;
	//执行命令消耗的时间 单位ns  (1/1,000,000,000 s)
	long long duration;
	//保存开始命令执行时的时间
	time_t time;
} slowlogEntry;

void slowlogInit(void);
void slowlogPushEntryIfNeeded(orbj **argv, int argc, long long duration);

void slowlogCommand(redisClient *c);
#endif
