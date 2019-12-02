#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>

int main()
{
	struct in_addr s;
	s.s_addr = inet_addr("192.168.1.1");
//	inet_aton("192.168.1.1", &s);
	printf("s.s_addr :0x%x\n", s.s_addr);

	printf("ip is :%s\n", inet_ntoa(s));

	return 0;
}
