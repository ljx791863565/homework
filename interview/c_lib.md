
//c语言常用的函数

#include <ctype.h>

int isalnum(int c);	//检查参数是否是英文字母或阿拉伯数字，在标准c中相当于 (isalpha(c)||isdigit(c))
					//若参数是字母或者数字，返回true，否则返回0

int isalpha(int c);	//检查参数是否是字母。不是返回0

int isascii(int c);	//检查参数是否是ASC||码 0-127，不是返回0

int iscntrl(int c);	//检查参数是否是ASC||控制码 0-30,不是返回0

int isdigit(int c);	//检查参数是否是数字。0-9，不是返回0

int islower(int c);	//检查参数是否为小写字母 a-z。不是返回0

int isupper(int c);	//检查参数是否为大写字母，A-Z ，不是返回0

int isspace(int c);	//检查参数是否为空字符。 不是返回0

int ispunct(int c);	//检查参数是否为标点符号或特殊符号(非空，数字，非字母)，不是返回0	


#include <stdlib.h>

double atof(const char *nptr);	//扫描参数字符串，将字符串转换成浮点型数。开始扫描是从第一个数字或正负号开始的，到'\0'结束的

int atoi(const char *nptr);		//将字符串转换成整型数

long atol(const char *nptr);	//将字符串转换成长整型数

char *gcvt(double number, size_t ndigits, char *buf);	//将double型数字转换成字符串，number是浮点数，ndigits表示显示的位数，buf保存转换后的字符串

double strtod(const char *nptr, char **endptr);	//将字符串转换成浮点数，endptr不为NULL，将遇上不和条件而终止的nptr中的字符传出，可输入为第一个不可转换的指针	可以等价于 atof();

long int strtol(const char *nptr, char **endptr, int base);	//将字符串转换成长整型数 base代表采用的进制 2-36，或者0

unsigned long int strtoul(const char *nptr, char **endptr, int base);	//将字符串转换成无符号的长整型数

int toascii(int c);	//将整型数转换成合法的ASC||字符 c会被转换为7位的unsigned char，第八位会被清除

int tolower(int c);	//将参数转换成小写字母返回，若参数是小写则直接返回

int toupper(int c);	//将参数转换成大写字母返回，若参数是大写则直接返回



void *calloc(size_t nmemb,size_t size);	//申请nmemb个size大小的相邻的内存快，返回指向首地址的指针，失败返回NULL，并把内存块内容清空

void *malloc(size_t size);	//返回申请size个内存大小的首地址。失败返回NULL

void free(void *ptr);	//释放内存
	
void *realloc(void *ptr, size_t size);	/传入一个malloc地址。改变申请内存大小,返回指针



#include <time.h>

time_t time(time_t *t);	//获取当前的时间，从1970到UTC的时间，以s做单位

struct tm *gmtime(const time_t *timep);	//时间转换函数，把日期和时间转换成格林威治（GMT）时间，以tm指针返回
/*
   struct tm{
		int tm_sec;		//0-59
		int tm_mun;		//0-59
		int tm_hour;	//0-23
		int tm_mday;	//0-31
		int tm_mon;		//1-12
		int tm_year;	//1900 -
		int tm_wday;	//weekday	0-6
		int tm_yday;	//0-356
		int tm_isdst;	//日光节约时间的旗帜
   };
   */

char *asctime(const struct tm *timeptr);	//将参数tm结构中的信息转换为现实世界的当地时间的时间日期表示方法，以字符串形式返回

char *ctime(const time_t *timep);	//将时间和日期以现实世界方法表示，并以字符串返回

struct tm *localtime(const time_t *timep);	//将参数所指的time_t 结构中的信息转换成当地时区的表示方法

time_t mktime(struct tm *timeptr);	//将结构数据转换成从1970/1/1/0时/0分/0秒算起至今UTC时间所经历的秒数

	
#include <sys/time.h>
#include <unistd.h>
int gettimeofday(struct timeval *tv, struct timezone *tz);	//将目前的时间由tv结构返回，当地时间信息放在timezone结构中
/	struct timeval{
		long tv_sec;
		long tv_usec;	
	};
	struct timezone{
		int tz_minuteswest;		//和GMT时间差了多少min
		int tz_dsttime;			//日光节约时间的状态，如下
		/*
			DST_NONE	//不使用
			DST_USA		//美国
			DST_AUST	//澳大利亚
			DST_WET		//西欧
			DST_MET		//中欧
			DST_EET		//东欧
			DST_CAN		//加拿大
			DST_GB		//大不列颠
			DST_RUM		//罗马尼亚
			DST_TUR		//土耳其
			DST_AUSTALT	//澳洲1986年以后			
		 */
	};

*/

int settimeofday(const struct timeval *tv, const struct timezone *tz);	//把目前时间设置成tv所指结构信息，当前时区信息设置成tz结构。只有root权限才能使用


#include <string.h>

char *index(const char *s, int c);	//返回s字符串出现第一个c字符的地址，没找到返回0

void *memset(void *s, int c, size_t n);	//s前n个字节以字符c写入

void *memcpy(void *dest, const void *src, size_t n);

void *memncpy(void *dest, const void *src, int c, size_t n);

int memcmp(const void *s1, const void *s2, int n);

int strcasecmp(const char *s1, const char *S2);	//忽略大小写比较字符串，相等返回0 

int strncasecmp(const char s2, const char *s2, size_t n);	//忽略大小写比较字符串前n个字符
	
void *memchr(const void *s, int c, size_t n);	//从字符串s开头搜索字符c，并返回第一个的地址，错误返回0



char *strcat(char *dest, const char *src);	//连接字符串

char *strncat(char *dest, const char *src, size_t n);

char *strchr(const char *src, int c);	//返回字符串中第一次出现字符c的地址

char *strrchr(const char *src, int c);	//返回字符串中最后一次出现字符c的地址

int strcmp(const char *Sl, const char *s2);	//比较字符串大小，想扽返回0

char strcpy(char *dest, const char *src);	//复制字符串

size_t strlen(const char *src);	//计算字符串src长度，不算'\0'

char *strstr(const char *haystack, const char *needle);	//在字符串中查找指定字符串并返回第一次出现的地址


