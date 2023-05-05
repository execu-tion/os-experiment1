#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include<stdlib.h>
#include <string.h>
#include <linux/list.h>
#include "os3.h"

int main(void){
    LIST_HEAD(pages);
    int pageNum = 0;
    printf("\033[32m程序总页数: \033[33m");
    scanf("%d",&pageNum);
    printf("\033[32m请给出驻留集大小: \033[33m");
    int pageKeep = 0;
    scanf("%d",&pageKeep);
    if( pageKeep > pageNum ){printf("\033[31m驻留集大于总页数,取驻留集等于总页数!\n");pageKeep=pageNum;}

    for(int i=0;i<pageNum;i++){
        int num = 0;
        printf("\033[37m第 %d 个页面号为：",i);
        scanf("%d",&num);
        Page *node = pageNew(num,i);
        list_add_tail(&(node->list),&pages);
    };
    printf("\033[32m请选择调度方法,[0:FIFO][1:CLOCK][2:LRU]:\033[37m");
    int choice;
    scanf("%d",&choice);
    schedule(&pages,pageNum,pageKeep,choice);
    return 0;
};
