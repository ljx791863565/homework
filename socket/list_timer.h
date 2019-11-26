#ifndef __LIST_TIME_H__
#define __LIST_TIME_H__

#include <time.h>
#define BUF_SIZE 64

class Util_time;

struct Client_data{
	struct sockaddr_in address;
	int sockfd;
	char buf[BUF_SIZE];
	Util_time *timer;
};

class Util_timer
{
public:
	Util_timer():prev(NULL), next(NULL){}
public:
	time_t expire;		
	void (*cb_func)(Client_data*);
	Client_data *user_data;
	Unil_timer *prev;
	Unil_timer *next;

};

class Sort_timer_list{
	Util_timer *head;
	Util_timer *tail;
public:
	Sort_timer_list():haed(NULL), tail(NULL){}
	~Sort_timer_list()
	{
		Util_timer *tmp;
		tmp = head;
		while (head){
			head = tmp->next;
			delete tmp;
			tmp = head;
		}
	}

	void add_timer(Util_timer *t, Util_timer *list_head){
		Util_timer *prev = head;
		Util_timer *tmp = prev->next;
		while (tmp){
			if (t->expire < tmp->expire){
				prev->next = t;
				t->next = tmp;
				tmp->prev = t;
				t->prev = prev;
				break;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		if (!tmp){
			prev->next = t;
			t->prev = prev;
			t->next = NULL;
			tail = t;
		}
	}

	void add_timer(Util_timer *t)
	{
		if (!t){
			return;
		}
		if (head == NULL){
			head = t;
			tail = t;
			return;
		}
		if (t->expire < head->expire){
			t->next = head;
			head->prev = t;
			head = t;
			return ;
		}
		add_timer(t, head);
	}

	void adjust_timer(Util_timer *t)
	{
		if (!t){
			return;
		}
		Util_timer *tmp = t->next;
		if (!tmp || (t->expire < tmp->expire)){
			return;
		}
		if (t == head){
			head = head->next;
			head->prev = NULL;
			t->next = NULL;
			add_timer(t, head);
		}else {
			t->next->prev = t->prev;
			t->prev->next = t->next;
			add_timer(t, t->next);
		}
	}

	void del_timer(Util_timer *t)
	{
		if (!t){
			return;
		}
		if (t == head && t == tail){
			delete t;
			head = NULL;
			tail = NULL;
			return;
		}
		if (t == head){
			head = head->next;
			head->prev = NULL;
			delete t;
			return;
		}
		if (t == tail){
			tail = tail->prev;
			tail->next = NULL;
			delete t;
			return;
		}
		t->prev->next = t->next;
		t->next->prev = t->prev;
		delete t;
		return;
	}

	void tick()
	{
		if (!head){
			return;
		}
		printf("tick timer\n");
		time_t cur = time(NULL);
		Util_timer tmp = head;
		while (tmp){
			if (cur < tmp->expire){
				break;
			}
			tmp->cu_func(tmp->User_data);	//do it something...
			head = tmp->next;
			if (head){
				head->prev = NULL;
			}
			delete tmp;
			tmp = head;
		}
	}
};
#endif
