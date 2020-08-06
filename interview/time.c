#include <stdio.h>
#include <time.h>

int main(int argc, const char *argv[])
{
	char tmp[20];
	time_t timep;
	time (&timep);
	printf("%s",ctime(&timep));
	system("date +\"%Y-%m-%d %H:%M:%S\"");

	struct tm *p;
	time(&timep);
	p = gmtime(&timep);
	printf("%d-%d-%d", (1900+p->tm_year), (1+p->tm_mon),p->tm_mday);
	printf(" %d:%d:%d\n", p->tm_hour, p->tm_min, p->tm_sec);
	char ttmpt[74] = {0};
	sprintf(ttmpt, "%d-%d-%d %d:%d:%d", (1900+p->tm_year), (1+p->tm_mon),p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	printf("%s\n", ttmpt);
	return 0;
}
