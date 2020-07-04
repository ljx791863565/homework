#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

#define HEADER1 "-------------------------------------STUDENT------------------------------------\n"
#define HEADER2 "	|number	|name	|Comp	|Math	|Eng	|Sum	|ave	|mici	|\n"
#define HEADER3 "	|		|		|		|		|		|		|		|		|"
#define FORMAT	"		|	%-10s|	%-15s|%4d|%4d|%4d|%4d| %.2f|%4d|\n"
#define DATA p->data.num, p->data.name, p->data.c_grade, p->data.m_grade, p->data.e_grade, p->data.total, p->data.ave, p->data.mingic
#define END		"--------------------------------------------------------------------------------\n"

int saveFlag = 0;
typedef struct Student
{
	char num[10];
	char name[15];
	int c_grade;
	int m_grade;
	int e_grade;
	int total;
	float ave;
	int mingic;
}ST;

typedef struct Node
{
	struct Student data;
	struct Node *next;
}Node, *Link;

void printHeader(void);
void printData(Node *node);
void stringInput(char *t, int len, char *notice);
int numberInput(char *notice);
void Disp(Link l);
Node *Locate(Link l, char *findmess, char *nameOrNum);
void Add(Link l);
void Insert(Link l);
void Qur(Link l);
void Del(Link l);
void Modify(Link l);
void Statistic(Link l);
void Sort(Link l);
void Save(Link l);

void menu(void);
void Wrong(void);

int main(int argc, const char *argv[])
{
	Link l;
	FILE *fp;
	int selete;
	char ch;
	int count = 0;
	Node *p, *r;
	l = (Node *)malloc(sizeof(Node));
	if (l == NULL) {
		perror("malloc");
		return -1;
	}

	l->next = NULL;
	r = l;

	fp = fopen("student.txt", "ab+");
	if (fp == NULL) {
		perror("fopen");
		return -1;
	}


	while (!feof(fp)) {
		p = (Node *)malloc(sizeof(Node));
		if (p == NULL) {
			perror("malloc");
			return -1;
		}
		if (fread(p, sizeof(Node), 1, fp) == 1) {
			p->next = NULL;
			r->next = p;
			r = p;
			count++;
		}
	}
	fclose(fp);
	printf("\n========>open file success, the total records number is :%d.\n", count);
	
	menu();
	while (1) {
		system("clear");
		menu();
		p = r;
		printf("\n   please enter your choice(0-9):");
		scanf("%d", &selete);
		if (selete == 0) {
			if (saveFlag == 1) {
				getchar();
				printf("\n========>whether save the modified record to file?(y/n):");
				scanf("%c", &ch);
				if ((ch == 'y') || (ch == 'Y')) {
					Save(l);
				}
			}
			printf("========>thank you for useness!\n");
			getchar();
			break;
		}
		switch (selete)
		{
			case 1:Add(l);break;
			case 2:Del(l);break;
			case 3:Qur(l);break;
			case 4:Modify(l);break;
			case 5:Insert(l);break;
			case 6:Statistic(l);break;
			case 7:Sort(l);break;
			case 8:Save(l);break;
			case 9:system("clear");
				   Disp(l);
				   break;
			default:
				   Wrong();
				   getchar();
				   break;
		}
	}
	return 0;
}

static void gotoXY(int x, int y)
{
	printf("%c[%d;%df]", 0x1B, y, x);
}
void menu(void)
{
	system("clear");
	printf("	The Student' Grade Management System\n");
	gotoXY(10, 8);
	printf("*************************************Menu*********************************\n");;
	gotoXY(10, 9);
	printf("	* 1 input record			2 delete record		\n");
	gotoXY(10, 10);
	printf("	* 3 search record			4 modify  record	\n");
	gotoXY(10, 11);
	printf("	* 5 insert record			6 count  record		\n");
	gotoXY(10, 12);
	printf("	* 7 sort record				8 save  record		\n");
	gotoXY(10, 13);
	printf("	* 9 display record			0 quit  record		\n");
	gotoXY(10, 14);
	printf("**************************************************************************\n");
}

void printData(Node *node)
{
	Node *p = node;
	printf(FORMAT, DATA);
}
void Wrong(void)
{
	printf("\n\n\n\n**********Error: input has wrong! press any key to continue***********\n");
	getchar();
}

static void NoFind(void)
{
	printf("\n===========>Not Find This Student!\n");
}

void Disp(Link l)
{
	Node *p = l->next;
	if (p == NULL) {
		printf("\n=========>Not Student Record\n");
		getchar();
		return;
	}
	printf("\n\n");
	printHeader();
	while (p) {
		printData(p);
		p = p->next;
		printf(HEADER3);
	}

	getchar();
}

void stringInput(char *t, int len, char *notice)
{
	char n[255];
	do {
		printf("%s", notice);
		scanf("%s", n);
		if (strlen(n) > len) {
			printf("\n exceed the required length.\n");
		}
	} while (strlen(n) > len);
	strcpy(t, n);
}
void printHeader()
{
	printf(HEADER1);
	printf(HEADER2);
}
int numberInput(char *notice)
{
	int t = 0;
	do {
		printf("%s", notice);
		if (t > 100 || t < 0) {
			printf("\n score must in [0-100].\n");
		}
	} while (t > 100 || t < 0);
	return t;
}

Node * Locate(Link l, char *findmess, char *nameOrNum)
{
	Node *r = NULL;
	if (strcmp(nameOrNum, "num") == 0) {
		r = l->next;
		while (r) {
			if (strcmp(r->data.num, findmess) == 0) {
				return r;
			}
			r = r->next;
		}
	}else if (strcmp(nameOrNum, "name") == 0) {
		r = l->next;
		while (r) {
			if (strcmp(r->data.name, findmess) == 0) {
				return r;
			}
			r = r->next;
		}
	}
	return NULL;
}

void Add(Link L)
{
	Node *p, *r, *s;
	char ch, flag = 0, num[10];
	r = L;
	s = L->next;

	system("clear");
//	Disp(L);
	while (r->next != NULL) {
		r = r->next;
	}

	while (1) {
		stringInput(num, 10, "input number:");
		flag = 0;
		if (strcmp(num, "0") == 0) {
			return;
		}
		s = L->next;
		while (s) {
			if (strcmp(s->data.num, num) == 0) {
				flag = 1;
				break;
			}
			s = s->next;
		}

		if (flag == 1) {
			getchar();
			printf("========>The Number %s is not existing, try again?(y/n):", num);
			scanf("%c", &ch);
			if ((ch == 'y') || (ch == 'Y')) {
				continue;
			}else {
				return;
			}
		}else {
			break;
		}

		p = (Node *)malloc(sizeof(Node));
		if (p == NULL) {
			perror("malloc");
			return;
		}

		strcpy(p->data.num, num);
		stringInput(p->data.name, 15, "Name:");
		p->data.c_grade = numberInput("C language score[0-100]:");
		p->data.m_grade = numberInput("Math score[0-100]:");
		p->data.e_grade = numberInput("English language score[0-100]:");
		p->data.total = p->data.c_grade + p->data.m_grade + p->data.e_grade;
		p->data.ave = (float)(p->data.total / 3);
		p->data.mingic = 0;

		p->next = NULL;
		r = p;
		saveFlag = 1;
	}
}

void Qur(Link L)
{
	int selete;
	char searchInput[20];
	Node *p = NULL;
	if (L->next == NULL) {
		system("clear");
		printf("\n=========>No Student Record\n");
		getchar();
		return;
	}
	system("clear");
	printf("\n===========>1 search by number  ==========>2 search by name\n");
	printf("please chouse [1, 2]:");
	scanf("%d", &selete);
	if (selete == 1) {
		stringInput(searchInput, 10 , "input the number:");
		p = Locate(L, searchInput, "num");
		if (p) {
			printHeader();
			printData(p);
			printf(END);
			printf("press any key to return");
			getchar();
		}else {
			NoFind();
			getchar();
		}
	}else if (selete == 2){
		stringInput(searchInput, 15, "input the name:");
		p = Locate(L, searchInput, "name");
		if (p) {
			printHeader();
			printData(p);
			printf(END);
			printf("press any key to return");
			getchar();
		}else {
			NoFind();
			getchar();
		}
	}else {
		Wrong();
		getchar();
	}
}

void Del(Link  L)
{
	int selete;
	char findmess[20];
	Node *p = NULL, *r = NULL;
	if (L->next == NULL) {
		system("clear");
		printf("\n=========>No Student Record\n");
		getchar();
		return;
	}
	system("clear");
	Disp(L);
	printf("\n===========>1 delete by number  ==========>2 delete by name\n");
	printf("please chouse [1, 2]:");
	scanf("%d", &selete);
	if (selete == 1) {
		stringInput(findmess, 10 , "input the number:");
		p = Locate(L, findmess, "num");
		if (p) {
			r = L;
			while (r->next != p) {
				r = r->next;
			}
			r->next = p->next;
			free(p);
			printf("\n ===========>delete success.\n");
			getchar();
			saveFlag = 1;
		}else {
			NoFind();
			getchar();
		}
	}else if (selete == 2){
		stringInput(findmess, 15, "input the name:");
		p = Locate(L, findmess, "name");
		if (p) {
			r = L;
			while (r->next != p) {
				r = r->next;
			}
			r->next = p->next;
			free(p);
			printf("\n ===========>delete success.\n");
			getchar();
			saveFlag = 1;
		}else {
			NoFind();
			getchar();
		}
	}else {
		Wrong();
		getchar();
	}
}

void Modify(Link L)
{
	Node *p = NULL;
	char findmess[20];
	if (L->next != NULL) {
		system("clear");
		printf("\n=========>No Student Record\n");
		getchar();
		return;
	}
	system("clear");
	printf("modify student recorder");
	Disp(L);
	stringInput(findmess, 10, "input the existing student number:");
	p = Locate(L, findmess, "num");
	if (p) {
		printf("Number:%s, \n", p->data.num);
		printf("Name:%s, ",p->data.name);
		stringInput(p->data.name, 15, "input new name:");
		printf("C language score:%d, ",p->data.c_grade);
		p->data.c_grade = numberInput("C language score[0-100]:");
		printf("Math score:%d, ",p->data.m_grade);
		p->data.m_grade = numberInput("Mathscore[0-100]:");
		printf("Englisg language score:%d, ",p->data.e_grade);
		p->data.e_grade = numberInput("English language score[0-100]:");
		p->data.total = p->data.c_grade + p->data.m_grade + p->data.e_grade;
		p->data.ave = (float)(p->data.total / 3);
		p->data.mingic = 0;
		printf("\n==========>modify success.\n");
		Disp(L);
		saveFlag = 1;
	}else {
		NoFind();
		getchar();
	}
}

void Insert(Link L)
{
	Link p, v, newinfo;
	char ch, num[10], s[10];
	int flag = 0;
	v = L->next;
	system("clear");
	Disp(L);
	while (1) {
		if (strcmp(v->data.num, s) == 0) {
			flag = 1;
			break;
		}
		v = v->next;
		if (flag == 1) {
			break;
		}else {
			getchar();
			printf("\n=========>the number: %s is not existing ,try again?(y/n):", s);
			scanf("%c", &ch);
			if ((ch == 'y') || (ch == 'Y')) {
				continue;
			}else {
				return;
			}
		}
	}

	stringInput(num, 10, "input new student number :");
	v = L->next;
	while (v) {
		if (strcmp(v->data.num, num) == 0) {
			printf("=========>sorry, the new number:'%s' is	existing.\b", num);
			printHeader();
			printData(v);
			printf("\n");
			getchar();
			return;
		}
		v = v->next;
	}
	newinfo = (Node *)malloc(sizeof(Node));
	if (newinfo == NULL) {
		perror("malloc");
		return;
	}
	strcpy(newinfo->data.num, num);
	stringInput(newinfo->data.name, 15, "Name:");
	printf("C language score:%d, ",p->data.c_grade);
	newinfo->data.c_grade = numberInput("C language score[0-100]:");
	printf("Math score:%d, ",newinfo->data.m_grade);
	newinfo->data.m_grade = numberInput("Mathscore[0-100]:");
	printf("Englisg language score:%d, ",newinfo->data.e_grade);
	newinfo->data.e_grade = numberInput("English language score[0-100]:");
	newinfo->data.total = newinfo->data.c_grade + newinfo->data.m_grade + newinfo->data.e_grade;
	newinfo->data.ave = (float)(newinfo->data.total / 3);
	newinfo->data.mingic = 0;
	newinfo->next = NULL;
	saveFlag = 1;
	p = L->next;

	while (L) {
		if (strcmp(p->data.num, s) == 0) {
			newinfo->next = p->next;
			p->next = newinfo;
			break;
		}
		p = p->next;
	}
	Disp(L);
	printf("\n\n");
	getchar();

}
void Statistic(Link L)
{
	Node *pc, *pe, *pm, *pt;
	Node *r = L->next;
	int countc,countm,counte;
	countc = countm = counte = 0;
	if (r == NULL) {
		system("clear");
		printf("\n==========>not student record.\n");
		getchar();
		return;
	}
	system("clear");
	Disp(L);
	pm = pc = pe = pt = r;
	while (r) {
		if (r->data.c_grade < 60) {
			countc++;
		}
		if (r->data.m_grade < 60) {
			countm++;
		}
		if (r->data.e_grade < 60) {
			counte++;
		}

		if (r->data.e_grade >= pe->data.e_grade) {
			pe = r;
		}
		if (r->data.c_grade >= pc->data.c_grade) {
			pc = r;
		}
		if (r->data.m_grade >= pm->data.m_grade) {
			pm = r;
		}
		if (r->data.total >= pt->data.total) {
			pt = r;
		}
		r = r->next;
	}
	printf("\n--------------------------------------Statistic----------------------------\n");
	printf("C language			< 60 :%d\n",countc);
	printf("Math				< 60 :%d\n",countm);
	printf("English language	< 60 :%d\n",counte);
	printf("-----------------------------------------------------------------------------\n");
	printf("the highest student by total score name :%s, total score:%d\n", pt->data.name, pt->data.total);
	printf("the highest student by C language score name :%s, score:%d\n", pc->data.name, pc->data.c_grade);
	printf("the highest student by Math score name :%s, score:%d\n", pm->data.name, pm->data.m_grade);
	printf("the highest student by Englisg language score name :%s, score:%d\n", pe->data.name, pe->data.e_grade);

	printf("please any key to return");
	getchar();

}

void Sort(Link L)
{
	Node *p, *r, *s;
	int i = 0;
	if (L->next == NULL) {
		system("clear");
		printf("\n==========>not student record.\n");
		getchar();
		return;
	}
	Link temp = (Node *)malloc(sizeof(Node));
	if (temp == NULL) {
		perror("malloc");
		return;
	}
	temp->next = NULL;

	system("clear");
	Disp(L);
	p = L->next;
	while (p != NULL) {
		s = (Node *)malloc(sizeof(Node));
		if (s == NULL) {
			perror("malloc");
			return;
		}
		s->data = p->data;
		s->next = NULL;
		r = temp;
		while (r->next != NULL && r->next->data.total >= p->data.total){
			r = r->next;
		}
		if (r->next == NULL) {
			r->next = s;
		}else {
			s->next = r->next;
			r->next = s;
		}
		p = p->next;
	}
	Disp(L);
	saveFlag = 1;
	printf("\n===========>sort complte.\n");
}

void Save(Link L)
{
	FILE *fp = fopen("student.txt", "wb");
	if (fp == NULL) {
		perror("fopen");
		return;
	}
	int count = 0;
	Node *p = L->next;
	while (p != NULL) {
		if (fwrite(p, sizeof(Node), 1, fp) == 1) {
			p = p->next;
			count++;
		}else {
			break;
		}
	}
	if (count > 0) {
		getchar();
		printf("\n\n\n\n=========>save file complte, total saves's record number is:%d\n", count);
		getchar();
		saveFlag = 0;
	}else {
		system("clear");
		printf("the current link is empty, no student record is saved.\n");
		getchar();
	}
	fclose(fp);
}
