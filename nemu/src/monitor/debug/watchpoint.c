#include "monitor/watchpoint.h"
#include "monitor/expr.h"


#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char expression[32])
{
	bool success = true;
	WP *p, *q;
	if (free_ == NULL)
	{
		assert(0);
	}
	else
	{
		p = free_;
	}
	free_ = free_->next;
	strcpy((*p).expr, expression);
	p->next = NULL;
	if (head == NULL)
	{
		head = p;
		p->next = NULL;
	}
	else
	{
		for (q = head; q->next !=  NULL; q = q->next);
		q->next = p;
		p->next = NULL;
	}
	for (q = head; q->next != NULL; q = q->next);
	(*q).value = expr(expression, &success);
	printf("watchpoint %d: %s\n", (*q).NO, expression);
	//printf("biaodashidezhi %d  %d\n", expr(expression, &success), (*q).value);
	return head;
}   

void free_wp(int no)
{	
	WP *wp = NULL;
	WP *p, *q;
	if (head == NULL)
	{
		assert(0);
 	}
 	else
 	{
		if ((*head).NO == no)
		{
			wp = head;
			head = head->next;
			wp->next = NULL;
		}
		else
		{
			for (q = head; q->next != NULL; q = q->next)
			{
				if ((*q->next).NO == no)
				{
					wp = q->next;
					if (q->next->next == NULL)
					{
						q->next = NULL;
						break;
					}
					else
					{
						q->next = q->next->next;
						break;
					}
					wp->next = NULL;
				}
			}
		}
		for (p = free_; p->next != NULL; p = p->next);
		p->next = wp;
		wp->next = NULL;		
	}
}	

int cal()
{
	bool success = true;
	int flag = 0;
	WP *p;
	uint32_t temp;
	for (p = head; p != NULL; p = p->next)
	{
		temp = expr((*p).expr, &success);
		if (temp != (*p).value)
		{
			printf("w%d old value: 0x%x\n", (*p).NO, (*p).value);
			(*p).value = temp;
			flag = 1;
			printf("w%d new value: 0x%x\n", (*p).NO, temp);
		}	
	}
	return flag;
}

