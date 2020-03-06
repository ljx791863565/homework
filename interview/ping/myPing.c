#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>

//保存发送包的状态值
typedef struct pingm_packet
{
	struct timeval tv_begin;	//发送时间
	struct timeval tv_end;		//接收到的时间
	short seq;					//序列号
	int flag;	//1表示已经发送但没有接收到回应 0表示接收到回应
}pingm_packet;

static pingm_packet *icmp_findpacket(int seq);
static unsigned short icmp_cksum(unsigned char *data, int len);
static struct timeval icmp_tvsub(struct timeval end, struct timeval begin);
static void icmp_statistics();
static void icmp_pack(struct icmp *icmph, int seq, struct timeval *tv, int length);
static int icmp_unpack(char *buf, int len);
static void *icmp_recv(void *argv);
static void *icmp_send(void *argv);
static void icmp_sigint(int signo);
static void icmp_usage();

static pingm_packet pingpacket[128];
#define K 1024
#define BUFSIZE 72

static unsigned char send_buff[BUFSIZE];
static unsigned char recv_buff[2*K];	
static struct sockaddr_in dest;
static int rawsock = 0;
static pid_t pid;
static int alive = 0;				//是否接收到退出信号
static short packet_send = 0;		//已经发送的数据包数量
static short packet_recv = 0;		//已经接收到的数据包数量
static char dest_str[80];			//目的主机字符串
static struct timeval tv_begin, tv_end, tv_interval;

static void icmp_usage()
{
	//ping加ip地址或域名
	printf("ping aaa.bbb.ccc.ddd\n");
}

//终端信号处理函数
static void icmp_sigint(int signo)
{
	alive = 0;
	gettimeofday(&tv_end, NULL);
	tv_interval = icmp_tvsub(tv_end, tv_begin);

	return ;
}

//统计数据结果函数
//打印全部ICMP发送的接收统计结果
static void icmp_statistics()
{
	long time = (tv_interval.tv_sec *1000) + (tv_interval.tv_usec /1000);
	printf("--- %s ping statistics ---\n", dest_str);
	printf("%d packets transmitted, %d received, %d%c packet loss, time %ld ms\n", packet_send, packet_recv,
			(packet_send-packet_recv)*100/packet_send, '%', time);
}

//查找数组中的标识函数
//查找合适的包的位置，当seq为1时，表示查找空包
//其他的值表示对应的seq包
static pingm_packet *icmp_findpacket(int seq)
{
	int i;
	pingm_packet *found = NULL;
	//查找包的位置
	if (seq == -1){
		for (i=0; i<128; i++){
			if (pingpacket[i].flag == 0){
				found = &pingpacket[i];
				break;
			}
		}
	}else if(seq >= 0){
		for (i=0; i<128; i++){
			if (pingpacket[i].seq == seq){
				found = &pingpacket[i];
				break;
			}
		}
	}
	return found;
}

/*
   校验函数
   TCP/IP协议栈使用的校验算法对16位的数据进行累加计算，并返回结果
   参数：data	数据
		 len	长度
	返回值：
		short计算结果
   */
static unsigned short icmp_cksum(unsigned char *data, int len)
{
	int sum = 0;	//计算结果
	int odd = len & 0x01;	//是否为奇数
	//将数字按照2字节为单位累加起来
	while (len & 0xfffe){
		sum += *(unsigned short *)data;
		data += 2;
		len -= 2;
	}
	//判断是否为奇数，若为奇数，ICMP报头会剩下一个字节
	if (odd){
		unsigned short tmp = (*(data) << 8)&0xff00;
		sum += tmp;
	}
	sum = (sum >> 16) + (sum & 0xffff);	//高低位相加
	sum += (sum >> 16);					//将溢出威加入

	return ~sum;
}

//进行ICMP头部校验
//设置ICMP报头
static void icmp_pack(struct icmp *icmph, int seq, struct timeval *tv, int length)
{
	unsigned char i = 0;
	//设置报头
	icmph->icmp_type = ICMP_ECHO;	//ICMP回显请求
	icmph->icmp_code = 0;
	icmph->icmp_cksum = 0;
	icmph->icmp_seq = seq;			//本报的序列号
	icmph->icmp_id = pid & 0xffff;	//填写PID
	for (i = 0; i < length; i++){
		icmph->icmp_data[i] = i;		//计算校验和
	}
	icmph->icmp_cksum = icmp_cksum((unsigned char *)icmph, length);
}

//解压接收到的包，并打印信息
static int icmp_unpack(char *buf, int len)
{
	int i, iphdrlen;
	struct ip *ip = NULL;
	struct icmp *icmp = NULL;
	int rtt;
	ip = (struct ip*)buf;			//IP报头
	iphdrlen = ip->ip_hl *4;		//IP头部长度
	icmp = (struct icmp*)(buf + iphdrlen);	//ICMP段的地址
	len -= iphdrlen;
	//判断长度是否为ICMP包
	if (len < 8){
		printf("ICMP packet\'s length is less than 8\n");
		return -1;
	}
	//ICMP类型为ICMP_ECHOREPLY并且为本进程PID
	if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == pid)){
		struct timeval tv_interval, tv_recv, tv_send;
		pingm_packet *packet = icmp_findpacket(icmp->icmp_seq);
		if (packet == NULL){
			return -1;
		}
		packet->flag = 0;	//取消标志
		tv_send = packet->tv_begin;		//获取本包的发送时间
		
		gettimeofday(&tv_recv, NULL);	//读取此时间
		tv_interval = icmp_tvsub(tv_recv, tv_send);	//计算时间差
		rtt = tv_interval.tv_sec *1000 + tv_interval.tv_usec / 1000;
		/*
		   打印结果包含
		   ICMP段的长度
		   源IP地址
		   包的序列号
		   TTL
		   时间差
		   */
		printf("%d bytes from %s: icmp_seq=%u ttl=%d rtt=%d ms\n", len, inet_ntoa(ip->ip_src),
				icmp->icmp_seq, ip->ip_ttl, rtt);
		packet_recv ++;			//接收包的数量+1
	}else{
		return -1;
	}
}

/*计算时间差
	参数：
		end:接收到的时间
		begin:开始发送的时间
	返回值：
		使用的时间
*/
static struct timeval icmp_tvsub(struct timeval end, struct timeval begin)
{
	struct timeval tv;
	tv.tv_sec = end.tv_sec - begin.tv_sec;
	tv.tv_usec = end.tv_usec - begin.tv_usec;

	//如果接收到的时间的usce小于发送的usce，从usec域借位
	if (tv.tv_usec < 0){
		tv.tv_sec --;
		tv.tv_usec += 1000000;
	}
	return tv;
}

//发送报文函数
static void *icmp_send(void *argv)
{
	//保存程序开始发送数据的时间
	gettimeofday(&tv_begin, NULL);
	while (alive){
		int size = 0;
//		printf("this is the icmp_send\n");
		struct timeval tv;
		gettimeofday(&tv, NULL);
		//在发送包状态数组中找到一个空闲位置
		pingm_packet *packet = icmp_findpacket(-1);
		if (packet){
			packet->seq = packet_send;
			packet->flag = 1;
			gettimeofday(&packet->tv_begin, NULL);
		}
		icmp_pack((struct icmp *)send_buff, packet_send, &tv, 64);
		//打包数据
		size = sendto(rawsock, send_buff, 64, 0, (struct sockaddr *)&dest, sizeof(dest));
		if (size < 0){
			perror("sendto");
			continue;
		}
		packet_send ++;
		//每隔1s发送一个ICMP回显请求包
		sleep(1);
	}
}

//接收ping主机的回复
static void *icmp_recv(void *argv)
{
	//轮询等待消息
	struct timeval tv;
	tv.tv_usec = 200;
	tv.tv_sec = 0;
	fd_set readfd;		//long类型的数组，每一个数组元素都能与一个打开的套接字(句柄)建立联系
//	printf("this is icmp_recv\n");
	while (alive){
		int ret = 0;
		FD_ZERO(&readfd);	//将set清零使得不含任何fd
		FD_SET(rawsock, &readfd);	//将fd加入readfd数组
		ret = select(rawsock+1, &readfd, NULL, NULL, &tv);
		switch (ret)
		{
			case -1:
				//错误
				break;
			case 0:
				//超时
				break;
			default:
				{
					//收到一个包
					int fromlen = 0;
					struct sockaddr from;
					//接收数据
					int size = recv(rawsock, recv_buff, sizeof(recv_buff), 0);
					if (errno == EINTR){
						perror("recvfrom");
						continue;
					}

					//解包
					ret = icmp_unpack(recv_buff, size);
					if (ret == 1){
						continue;
					}
				}
				break;
		}
	}
}

//设置ICMP发送报文头部

/*
   struct icmp
   {
		u_int8_t icmp_type;	//消息类型
		u_int8_t icmp_code;	//消息类型的子码
		u_int16_t icmp_cksum;	//校验和
		union
		{
			//显示数据报
			struct ih_idseq	
			{
				u_int16_t icd_id;	//数据报ID
				u_int16_t icd_seq;	//数据报序列号
			}ih_idseq;
		}icmp_hun;
#define icmp_id icmp_hun.ih_idseq.icd_id;
#define icmp_seq icmp_hun.ih_idseq.icd_seq;
		union
		{
			u_int8_t id_data[1];	//数据
		}icmp_dun;
#define icmp_data icmp_dun.id_data;
   }
   */

int main(int argc, char const *argv[])
{
	struct hostent *host = NULL;
	struct protoent *protocol = NULL;
	char protoname[] = "icmp";
	unsigned long inaddr = 1;
	int size = 128*K;

	if (argc < 2){
		icmp_usage();
		return -1;
	}

	//获取协议类型
	protocol = getprotobyname(protoname);	//返回对应于给定协议名的相关信息
	if (protocol == NULL){
		perror("getprotobyname()");
		return -1;
	}

	//复制目的地址字符串
	memcpy(dest_str, argv[1], strlen(argv[1])+1);
	memset(pingpacket, 0, sizeof(pingm_packet)*128);

	rawsock = socket(AF_INET, SOCK_RAW, protocol->p_proto);
	if (rawsock < 0){
		perror("socket");
		return -1;
	}

	pid = getpid();
	//增大接收缓冲区，防止接受包被覆盖
	setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	bzero(&dest, sizeof(dest));

	//获取目的地的IP地址
	dest.sin_family = AF_INET;
	inaddr = inet_addr(argv[1]);
	//输入的是DNS地址
	if (inaddr == INADDR_NONE){
		host = gethostbyname(argv[1]);
		if (host == NULL){
			perror("gethostbyname");
			return -1;
		}
		//将地址复制到dest
		memcpy((char *)&dest.sin_addr, host->h_addr, host->h_length);
	}else{
		memcpy((char *)&dest.sin_addr, &inaddr, sizeof(inaddr));
	}

	inaddr = dest.sin_addr.s_addr;
	printf("PING %s (%ld.%ld.%ld.%ld) 56(84) bytes of data.\n", dest_str, (inaddr&0x000000ff)>>0, 
			(inaddr&0x0000ff00)>>8, (inaddr&0x00ff0000)>>16, (inaddr&0xff000000)>>24);

	//截取SIGINT信号，将icmp_sigint挂接上
	signal(SIGINT, icmp_sigint);

	/*
	   发送数据并接收回应
	   建立两个线程，一个用于发送数据，另一个用于接收响应数据
	   主线程等待两个线程运行完毕再进行
	   最后对结果进行统计并打印
	   */
	alive = 1;
	pthread_t send_id, recv_id;
	int err = 0;
	err = pthread_create(&send_id, NULL, icmp_send, NULL);
	if (err < 0){
		perror("pthread_create");
		return -1;
	}
	err = pthread_create(&recv_id, NULL, icmp_recv, NULL);
	if (err < 0){
		perror("pthread_create");
		return -1;
	}
	pthread_join(send_id, NULL);
	pthread_join(recv_id, NULL);

	close(rawsock);
	icmp_statistics();

	return 0;
}	
