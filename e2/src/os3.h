#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <linux/list.h>

typedef void(*Method)(struct list_head*,int);
typedef struct item
{
	int num;		//页号
	int time;		//等待时间，LRU算法会用到这个属性（如果要使用clock算法，还要加一个使用位）
    int Wem;        //标志位
    struct list_head list;
}Page;

/*patch kernel list for specific use*/
#define __replace(old,new) {\
    (void)((old) == (struct list_head*)(old) );\
    (void)((new) == (struct list_head*)(new) );\
    (new)->next = (old)->next; \
    (old)->next->prev = new;\
    (new)->prev = (old)->prev; \
    (old)->prev->next = new;\
}\
//重排链表
#define list_reverse(pos,head,member)  do{\
    LIST_HEAD(__NEW);\
    typeof(pos) cur;\
    list_for_each_entry_safe(pos,cur,head,member){ list_del(&(pos->member));list_add(&(pos->member),&__NEW);}\
    __replace(&__NEW,head);\
}while(0);\

Page *pageNew(int num,int seq);
void disp(struct list_head* head);
void schedule(struct list_head *pages,int pageNum,int pageKeep,int method);