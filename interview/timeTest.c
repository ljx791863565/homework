#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<error.h>
#include <stdlib.h>
 
int main(void)
{
 
    time_t Nowt=0;
	time_t i=0;
	char *buf=NULL;
	char Buf[50];
	struct tm *p;			//struct tm变量
	char outstr[200];
	
	i = time(&Nowt);			//自1970来到现在一共过去了几秒
	printf("1970到目前经历的秒数为：\n");
	printf("i=%ld\n",i);
	puts("\n");
	
	printf("1970到目前经历的秒数转成成字符串格式显示：\n");
	buf = ctime(&Nowt);			//得到一个可视化时间字符串
	printf("buf=%s",buf);
	ctime_r(&Nowt,Buf);			//可重入的版本，指定输出的位置，而不是内核随机位置输出
	printf("Buf=%s",Buf);
	puts("\n");
	
	p=localtime(&Nowt);			//获取本地时区的时间
	printf("1970到目前经历的秒数通过localtime转换成struct tm结构体时间：\n");
	//1900年为基准，月份是0-11月
	printf("%d年%d月%d日%d时%d分%d秒\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);	
	localtime_r(&Nowt,p);			//可重入的版本，指定输出的位置，而不是内核随机位置输出
	printf("%d年%d月%d日%d时%d分%d秒\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	puts("\n");
	
	p=gmtime(&Nowt);
	printf("1970到目前经历的秒数通过gmtime转换成struct tm结构体时间：\n");
	//格林尼治时间值与北京时间小时相差8小时。
	printf("%d年%d月%d日%d时%d分%d秒\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);		
	gmtime_r(&Nowt,p);			//可重入的版本，指定输出的位置，而不是内核随机位置输出
	printf("%d年%d月%d日%d时%d分%d秒\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	puts("\n");
	
	buf = asctime(p);			//将struct tm时间结构体转换成字符串格式输出。
	printf("将struct tm结构体时间转换成字符串格式时间显示：\n");
	printf("Buf=%s",buf);
	asctime_r(p,Buf);			//可重入的版本，指定输出的位置，而不是内核随机位置输出
	printf("Buf=%s",Buf);
	puts("\n");
	
	strftime(outstr,sizeof(outstr),"%F %j",p);			//选择输出什么格式  %F 年-月-日  %j 十进制表示的每年的第几天
	printf("将struct tm转换成特定格式的时间规则样式显示：\n");
	printf("Result string is \"%s\"\n", outstr);
	puts("\n");
	
	i = mktime(p);				//将struct tm结构体反转，变为自1970年到现在经过的秒数
	printf("将struct tm反转为1970年到现在经过的秒数\n");
	printf("i=%ld\n",i);
	
	return 0;
 
}   

