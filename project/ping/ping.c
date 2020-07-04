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
#include <netdb.h>
#include <string.h>
#include <pthread.h>

#define K 1024
#define BUFFSIZE 72
//发送包
typedef struct pingm_packet{
	struct timeval tv_begin;	//保存发送时间
	struct timeval tv_end;		//保存接收时间
	short seq;			//序列号
	int flag;			//1：已经发送但没有回应 0：接收到并回应
}pingm_packet;

static pingm_packet pingpacket[128];
static char dest_str[80];		//目的主机ip地址
static struct sockaddr_in dest;
static int rawsock = 0;		//fd
static pid_t pid;
static int alive = 0;		//线程退出的信号 0：接收到 1：未接收到
static short packet_send = 0;	//已经发送的数据包数量
static short packet_recv = 0;	//已经接收到的数据包数量
static unsigned char send_buff[BUFFSIZE];
static unsigned char recv_buff[2 * K];
static struct timeval tv_begin, tv_end, tv_interval;

static pingm_packet* icmp_findpacket(int seq);		
static unsigned short icmp_cksum(unsigned char *data, int len);
static struct timeval icmp_tvsub(struct timeval end, struct timeval begin);
static void icmp_statistics();
static void icmp_pack(struct icmp *icmph, int seq, struct timeval *tv, int length);
static int icmp_unpack(char *buff, int len);
static void* icmp_send(void *arg);
static void* icmp_recv(void *arg);
static void icmp_sigint(int signo);
static void usage();


static void icmp_usage()
{
	printf("ping IP \n");
}

static struct timeval icmp_tvsub(struct timeval end, struct timeval begin)
{
	struct timeval tv;
	tv.tv_sec = end.tv_sec - begin.tv_sec;
	tv.tv_usec = end.tv_usec - begin.tv_usec;

	if (tv.tv_usec < 0){
		tv.tv_sec --;
		tv.tv_usec += 1000000;
	}
	return tv;
}

//信号处理函数
static void icmp_sigint(int signo)
{
	alive = 0;
	gettimeofday(&tv_end, NULL);
	tv_interval = icmp_tvsub(tv_end, tv_begin);

	return;
}

//统计结果，并打印出结果
static void icmp_statistics()
{
	long time = (tv_interval.tv_sec *1000) + (tv_interval.tv_usec / 1000);
	printf("--- %s ping statistics -- \n",dest_str);
	printf("%u packets transmitted, %u received, %d%c packet lost, time %ld ms\n",
			packet_send, packet_recv, (packet_send - packet_recv)/packet_send*100, '%', time);
}

/*
 * 查找数组中的标识函数
 * 查找合适的包的位置，seq=-1，表示空包,其他值表示对应的seq包
 */
static pingm_packet* icmp_findpacket(int seq)
{
	int i = 0;
	pingm_packet *found = NULL;

	if (seq == -1){
		for (i=0; i<128; i++){
			if (pingpacket[i].flag == 0){
				found = &pingpacket[i];
				break;
			}
		}
	}else if (seq >=0){
		for (i=0; i<128; i++){
			if (pingpacket[i].seq == seq){
				found = &pingpacket[i];
			}
		}
	}

	return found;
}

/*
 * 校验函数
 * TCP/IP协议栈使用的检验算法对16位的数据进行累加计算，并返回结果
 * data 数据 len 数据长度
 * 返回值 unsigned short类型累计结果
 */
static unsigned short icmp_cksum(unsigned char *data, int len)
{
	int sum = 0;
	int odd = len & 0x01;		//奇数
	//将数字按照2字节为单位累加
	while (len & 0xfffe){
		sum += *(unsigned short *)data;
		data += 2;
		len -= 2;
	}

	//判断是否为奇数
	if (odd){
		unsigned short tmp = (*(data) << 8) & 0xff00;
		sum += tmp;
	}

	sum = (sum >> 16) + (sum & 0xffff);		//高低位相加
	sum += (sum >> 16);						//计算溢出

	return ~sum;
}

/*
 * TCP头部校验
 * 设置ICMP报头
 */
static void icmp_pack(struct icmp* icmph, int seq, struct timeval *tv, int length)
{
	unsigned char i = 0;
	icmph->icmp_type = ICMP_ECHO;
	icmph->icmp_code = 0;
	icmph->icmp_cksum = 0;
	icmph->icmp_seq = seq;
	icmph->icmp_id = pid & 0xffff;
	for (i = 0; i < length; i++){
		icmph->icmp_data[i] = i;
	}
	icmph->icmp_cksum = icmp_cksum((unsigned char *)icmph, length);
}

//解压接收到的包，打印信息
static int icmp_unpack(char *buff, int len)
{
	int i, iphdrlen;
	struct ip *ip = NULL;
	struct icmp *icmp = NULL;
	int rtt;
	ip = (struct ip*)buff;
	iphdrlen = ip->ip_hl * 4;
	icmp = (struct icmp*)(buff + iphdrlen);
	len -= iphdrlen;
	if (len < 8){
		printf("ICMP packet\'s length is less than 8\n");
		return -1;
	}

	//ICMP类型为ICMP_ECHOREPLY并且为本进程ID
	if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == pid)){
		struct timeval tv_interval, tv_recv, tv_send;
		pingm_packet *packet = icmp_findpacket(icmp->icmp_seq);
		if (packet == NULL){
			return -1;
		}
		packet->flag = 0;
		tv_send = packet->tv_begin;				//获得包发送时间
		
		gettimeofday(&tv_recv, NULL);		//获得当下时间
		tv_interval = icmp_tvsub(tv_recv, tv_send);
		rtt = tv_interval.tv_sec *1000 + tv_interval.tv_usec / 1000;	//ms

		/*
		 * 结果打印
		 * ICMP段长度 源IP地址 包序列号 TTL 时间差
		 */
		printf("%d bytes from %s: icmp_seq=%d ttl=%d rtt=%d ms\n", len, inet_ntoa(ip->ip_src),
				icmp->icmp_seq, ip->ip_ttl, rtt);
		packet_recv ++;
	}else {
		return -1;
	}
}
	
//发送报文
void *icmp_send(void *arg)
{
	gettimeofday(&tv_begin, NULL);
	while (alive){
		int size = 0;
		printf("this is the icmp_send\n");
		struct timeval tv;
		gettimeofday(&tv, NULL);
		pingm_packet *packet = icmp_findpacket(-1);
		if (packet){
			packet->seq = packet_send;
			packet->flag = 1;
			gettimeofday(&packet->tv_begin, NULL);
		}
		//打包数据
		icmp_pack((struct icmp*)send_buff, packet_send, &tv, 64);
		size = sendto(rawsock, send_buff, 64, 0, (struct sockaddr *)&dest, sizeof(dest));
		if (size < 0){
			perror("sendto()");
			continue;
		}
		packet_send ++;
		sleep(1);		//每1s发送一个ICMP回复请求包
	}
}

//接收报文
void *icmp_recv(void *arg)
{
	struct timeval tv;
	tv.tv_usec = 200;
	tv.tv_sec = 0;
	//log类型的数组 每一个数组元素都能与一个套接字建立联系
	fd_set readfd;
	while (alive){	
		printf("this is the icmp_send\n");
		int ret = 0;
		FD_ZERO(&readfd);
		FD_SET(rawsock, &readfd);
		//每200usec轮询一次readfd，查看消息
		ret = select(rawsock+1, &readfd, NULL, NULL, &tv);
		int size = 0;
		switch (ret){
			case -1:
				break;
			case 0:
				break;
			default:
				//int fromlen = 0;
				//struct sockaddr from;
				size = recv(rawsock, recv_buff, sizeof(recv_buff), 0);
				if (errno == EINTR){
					perror("recvfrom()");
					continue;
				}

				ret = icmp_unpack(recv_buff, size);
				if (ret == 1){
					continue;
				}
				break;
		}
	}
}
int main(int argc, char **argv)
{

	if (argc < 2){
		icmp_usage();
		return -1;
	}

	struct protoent *protocol = NULL;
	char protoname[] = "icmp";
	protocol = getprotobyname(protoname);		//获取协议类型
	if (protocol == NULL){
		perror("getprotobyname()");
		return -1;
	}
	
	//复制目的地址字符串
	memcpy(dest_str, argv[1], strlen(argv[1])+1);
	memset(pingpacket, 0, sizeof(pingm_packet)*128);

	//SOCKET
	rawsock = socket(AF_INET, SOCK_RAW, protocol->p_proto);
	if (rawsock < 0){
		perror("socket()");
		return -1;
	}

	pid = getpid();

	//增大接收缓冲区，防止接收包被覆盖
	int size = 128 * K;		
	setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	memset(&dest, 0, sizeof(dest));

	unsigned long inaddr = 1;
	dest.sin_family = AF_INET;
	inaddr = inet_addr(argv[1]);
	
	struct hostent *host = NULL;
	//输入的地址时候DNS合法地址
	if (inaddr == INADDR_NONE){
		host = gethostbyname(argv[1]);		
		if (host == NULL){
			perror("gethostbyname()");
			return -1;
		}
		memcpy((char*)&dest.sin_addr, host->h_addr, host->h_length);
	}else {
		memcpy((char*)&dest.sin_addr, &inaddr, sizeof(inaddr));
	}
	
	inaddr = dest.sin_addr.s_addr;
	printf("PING %s (%ld.%ld.%ld.%ld) 56(84) bytes of data.\n", dest_str, (inaddr&0x000000ff)>>0, (inaddr&0x0000ff00)>>8, (inaddr&0x00ff0000)>>16, (inaddr&0xff000000)>>24);

	//截取SIGNAL信号，连接icmp_sigint
	signal(SIGINT, icmp_sigint);
	
	/*
	 * 发送数据并接收回应
	 * 建立两个线程分别用于发送和接收
	 * 主线程等待接收和发送的结果
	 * 打印
	 */
	alive = 1;
	pthread_t send_id, recv_id;
	int ret = pthread_create(&send_id, NULL, icmp_send, NULL);
	if (ret < 0){
		perror("pthread_create()");
		return -1;
	}
	ret = pthread_create(&recv_id, NULL, icmp_recv, NULL);
	if (ret < 0){
		perror("pthread_create()");
		return -1;
	}

	pthread_join(send_id, NULL);
	pthread_join(recv_id, NULL);

	close(rawsock);
	icmp_statistics();
}
