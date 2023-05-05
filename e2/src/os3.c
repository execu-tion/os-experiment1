#include "os3.h"
struct list_head *clockptr = NULL;
void vfree(struct list_head *head){
    Page *pi = NULL,*cur=NULL;
    list_for_each_entry_safe(pi,cur,head,list){
        list_del(&(pi->list));
        free(pi);
        if(list_empty(head)){return;};
    };
};
Page *hasPage(struct list_head *head,int num){
    Page *pi = NULL;
    list_for_each_entry(pi,head,list){
        if( pi->num == num ) return pi;
    };
    return NULL;
};
Page *minTime(struct list_head *head){
    Page *pi = NULL;
    int t = 10001;
    Page *min = NULL;
    list_for_each_entry(pi,head,list){
        if( pi->time < t ){
            t = pi->time;
            min = pi;
        };
    };
    return min;
};

Page *pageNew(int num,int seq){
    Page *new = (Page*)calloc(1,sizeof(Page));
    new->num = num;
    new->time = seq + time(NULL)%10000;//time只能表示到秒，如果一秒内输入两个数
                                      //会有逻辑错误，所以加上输入顺序，以保证每个数的时间都不同
                                      //所以time并不严格是time，只是表示一个时间上的顺序
    new->Wem = 1;
    return new;
};

void disp(struct list_head* head){
    Page *pi;
    list_for_each_entry(pi,head,list){
        if( pi->num == -1) continue;
        printf("\033[32m\t[num:%d time:%d wem:%d]",pi->num,pi->time,pi->Wem);
    };
    printf("\033[37m \n");
};
int FIFO(struct list_head *head,Page *page){
    Page *pi = hasPage(head,page->num);
    if( pi == NULL ){
        Page *i = minTime(head);
        i->num = page->num;
        i->time = page->time;
        free(page);
        return 1;
    }else{
        return 0;
    };
};
int CLOCK(struct list_head *head,Page *page){
    Page *pos = hasPage(head,page->num);
    if( pos != NULL ){
        pos->Wem = 1;
        pos->time = page->time;
        free(page);
        return 0 ;
    };
    struct list_head* it = clockptr;
    while( 1 ){
        if( it == head ) { it = it->next;continue; };
        pos = list_entry(it,Page,list);
        if( pos->Wem == 0 ){//替换
            pos->num = page->num;
            pos->time = page->time;
            pos->Wem = 1;
            free(page);
            clockptr = (pos->list.next);
            return 1;
        }else{//清零
            pos->Wem = 0;
            disp(head);
            it = it->next;
        };
    };
};
int LRU(struct list_head *head,Page *page){
    Page *pi = hasPage(head,page->num);
    if( pi == NULL ){
        Page *i = minTime(head);
        i->num = page->num;
        i->time = page->time;
    }else{
        pi->time = page->time;
        return 0;
    };
    free(page);
    return 1;
};

static int(*METHODS[3])(struct list_head*,Page*)={FIFO,CLOCK,LRU};

void schedule(struct list_head *pages,int pageNum,int pageKeep,int method){
    LIST_HEAD(head);
    clockptr = &head;
    pages = pages->next;
    Page *pi = NULL;
    int num = 0;
    for(int i = 0;i<pageNum;i++ ){
        Page *pi = list_entry(pages,Page,list);
        pages = pages->next;
        list_del(&(pi->list));
        if( i<pageKeep ){ 
            Page *pn = hasPage(&head,pi->num); 
            if( pn == NULL ){
                list_add_tail(&(pi->list),&head);
            }else{
                (method!=0)&&(pn->time = pi->time);
                pn->Wem = 1;
                free(pi);
                pageKeep++;
            };    
        }else{
            if( (*METHODS[method])(&head,pi) == 1) num++;
        };
        disp(&head);
    };
    printf("缺页中断次数：%d,缺页率:%f%%\n",num,((float)num)/pageNum*100);
    vfree(&head);
};
