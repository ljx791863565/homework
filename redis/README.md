# 基本数据结构
* zmalloc.h/zmalloc.c		内存分配
* sds.h/sds.c				动态字符串
* adlist.h/adlist.c			双端链表
* dict.h/dict.c				字典
* redis.h					zskiplist 结构和 zskiplistNode 结构 以及 t_zset.c 中所有以 zsl 开头的函数
							比如 zslCreate  zslInsert  zslDeleteNode 
* hyperloglog.c				基数统计	hllhdr结构, 所有hll开头的函数


# 内存编码结构
* intser.h/intset.c			整数集合数据结构
* ziplist.h/ziplist.c		压缩列表数据结构


# Redis数据类型实现
* object.c					对象系统
* t_string.c				字符串键
* t_list.c					列表键
* t_hash.c					散列键
* t_set.c					集合键
* t_zset.c					有序集合键   除zsl开头的函数之外的所有函数
* hyperloglog.c				HyperLogLog键 所有pf开头的函数

# 熟悉Redis数据库实现
* redis.h					数据库实现 redisDb结构,db.c文件
* notify.c					通知功能
* rdb.c						RDB持久化
* aof.c						AOF持久化

# 独立功能模块实现
* redis.h					发布和订阅 pubsubPattern结构, pubsub.c文件
* redis.h					事务   multiState结构,multiCmd结构, multi.c文件

# 客户端和服务器代码实现
* ae.c/ae_epoll.c/ae_evport.c/ae_kqueue.c/ae_select.c		事务处理模块
* anet.c/networking.c										网络连接库
* redis.c													服务器端
* redis-cli.c												客户端
* scripting.c												lua脚本
* slowlog.c													慢查询
* monitor.c													监视

# Redis多机部分代码实现
* replication.c												复制功能
* sentinel.c												Redis Sentinel
* cluster.c													集群

# 测试相关代码
* memtest.c													内存检测
* redis_benchmark.c											性能测试的实现
* redis_check_aof.c											更新日志检查的实现

# 一些工具类文件
* bitops.c					DETBIT SETBIT等二进制未操作命令实现
* debug.c					调试
* endianconv.c				不同系统的高低位转换
* help.h					命令的提示信息
* lzf_c.c lzf_d.c			压缩算法
* rand.c					产生随机数
* release.c					发布时使用相关
* sha1.c					sha加密算法实现
* util.c					通用工具方法
* crc64.c					循环冗余校验
* sort.c					SORT命令实现
* bio.c						background I/O	开启后台线程
* latency.c					延迟类
* migrate.c					命令迁移类,包括命令的还原迁移
* pqsort.c					排序算法类
* rio.c						Redis定义的一个I/O类
* syncio.c					用于同步Socket和文件I/O操作
