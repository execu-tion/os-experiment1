#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
/*--------------include-------------*/
#include <string.h>
#include "os2.h"
/*----------initial param-----------*/
const int memsize = 1024;//内存大小设置为1024
LIST_HEAD(memMap);       //整个内存
/*----------------------------------*/
int main(void){
    Lnode *memall =(Lnode*)malloc(sizeof(Lnode));
    memall->size=0;memall->size=1024;memall->state=48;
    list_add(&(memall->list),&memMap);
    while (1)
    {
        printf("input usage[ alloc[0] free[1] disp[p] ]: ");
        char choice;
        scanf("%c",&choice);getchar();
        switch (choice)
        {
        case 48:
            alloc_func( &memMap );
            break;
        case 49:
            free_func(&memMap);
            break;
        case 112:
            listDisp(&memMap);
            break;
        case 113:
            return 0;
        case 's'+0 :
            memsmove(&memMap);
            break;
        default:
            printf("\033[31m  %d Invalid value\033[37m\n",choice);
            break;
        };
    };
    return 0;
};
