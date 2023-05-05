#include<stdio.h>
#include <linux/list.h>
#include <stdlib.h>

typedef struct Lnode{
    int startAddress;
    int size;
    int state;
    /*方便管理*/
    struct list_head list;
}Lnode;
void listDisp(struct list_head *head){
    system("clear");
    int i = 0;
    Lnode *pos;
    printf("=========================================================\n");
    printf("Lnum\tstart\t|size\t|state\n");
    printf("=========================================================\n");
    list_for_each_entry(pos, head, list){
		printf("%d\t%d\t|%d\t|%c\t\n",i,pos->startAddress,pos->size,pos->state);
        i++;
	};
    printf("=========================================================\n");
};
void memsmove(struct list_head *head){
    Lnode *cur = NULL;
    Lnode *new = (Lnode*)malloc(sizeof(Lnode));
    Lnode *pur = NULL;
    int freemem = 0;
    //计算空闲大小，并移除页表
    list_for_each_entry(cur,head,list){
        if( cur->state == 48 ) freemem+= cur->size;
    };
    list_for_each_entry_safe(cur,pur,head,list){
        if( cur->state == 48 ){
            list_del(&(cur->list));
            free(cur);
        };
    };
    //重新计算
    int start = 0;
    list_for_each_entry(cur,head,list){
        if( cur->startAddress != start ){
            cur->startAddress = start;
            start += cur->size;
        };
    };

    new->startAddress = start;
    new->size = freemem;
    new->state = '0'+0 ;
    list_add(&(new->list),head->prev);
};
short error = 0;

void memmerge(struct list_head *head){
    Lnode *cur = NULL;
    Lnode *pur = NULL;
    list_for_each_entry(cur,head,list){
        if( cur->state == 49 ) continue;
        list_for_each_entry(pur,&(cur->list),list){
            if( pur->state == 49 ) break;
            else if( pur->state == 48 ){
                cur->size += pur->size;
                list_del( &(pur->list) );
                free(pur);
                pur = cur;
            }else{
                break;
            };
        };
        continue;
    };
    error ++;
    if( error > 1 ){
        memsmove(head);
    };
    printf("\033[31mNO MEM!\033[33m\ndo sort out func!you can retry you attempt!\033[37m\n");
};

void free_func(struct list_head *head){
    printf("input Lnode num: ");
    int lnum = 0;
    Lnode *pos = NULL;
    scanf("%d",&lnum);getchar();
    int i = 0;
    list_for_each_entry(pos,head,list){
        if( i == lnum ){
            pos->state = 48;
            listDisp(head);
            return;
        };
        i++;
    };
    printf("\033[31mno Lnode!\033[37m\n");
    return ;
};

void best_alloc(struct list_head *head, int size){
    Lnode *node = (Lnode*)calloc(1,sizeof(Lnode));
    node->size = size;
    node->state = 49;
    Lnode *cur = NULL;
    Lnode *pur = NULL;
    if(size==0) return ;
    
    int bestfit = 1024;
    list_for_each_entry(pur,head,list){
        if( pur->state == 48 && size <= pur->size && pur->size < bestfit ){//no use
            bestfit = pur->size;
            cur = pur;
        };
    };
    if( pur == NULL ){
        memmerge( head );
    }else{
        node->startAddress = cur->startAddress;
        cur->startAddress += size;
        cur->size -= size;
        list_add_tail(&node->list,&cur->list); 
        if( cur->size == 0 ){
            list_del( &(cur->list) );
            free(cur);
        };           
        listDisp(head);
        error = 0;
        return ;
    };
    memmerge( head );
};

void first_method(struct list_head *head, int size){
    Lnode *node = (Lnode*)calloc(1,sizeof(Lnode));
    node->size = size;
    node->state = 49;
    Lnode *cur = NULL;
    if(size==0) return ;
    list_for_each_entry(cur,head,list){
        if( cur->state == 48 && size <= cur->size ){//no use
            node->startAddress = cur->startAddress;
            cur->startAddress += size;
            cur->size -= size;
            list_add_tail(&node->list,&cur->list); 
            if( cur->size == 0 ){
                list_del( &(cur->list) );
                free(cur);
            };  
            listDisp(head);
            error = 0;
            return;
        };
    };
    memmerge( head );
};
void loge(struct list_head *head, int size){
    printf("\033[33mInvalid method, default use [0] for First method\033[37m\n");
    return ;
};
void (*methods[3])(struct list_head*, int) = {first_method,best_alloc,loge};

void alloc_func(struct list_head *head){
    printf("alloc size[1~1024]: ");
    int size = 0;
    scanf("%d",&size);getchar();
    printf("%d\n",size);
    printf("alloc method[[first 0]\\[best 1]: ");
    int method = 0;
    scanf("%d",&method);getchar();
    (*(methods[method]))(head,size);
/*  switch (method)
    {
    case 0:
        first_method(head,size);
        break;
    case 1:
        best_alloc(head,size);
        break;
    default:
        printf("\033[33mInvalid method, default use [0] for First method\033[37m\n");
        break;
    };*/
};
