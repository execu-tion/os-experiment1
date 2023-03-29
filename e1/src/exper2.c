#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pthread.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>
#include<string.h>
typedef struct Queue Queue;
/*LILO structure implemention*/
typedef struct Queue{
	char queue[5];
	int num;
	void (*append)(Queue*,char);
	char (*get)(Queue*);
}Queue;
void attach(Queue *Que,char ch){
	*( Que->queue + Que->num )= ch;
	Que->num  += 1;
};
char get(Queue *Que){
	char ch = *Que->queue ; 
	char temp[5] ;
	strcpy(temp,(Que->queue +1 )) ;
	memset(Que->queue,0,5*sizeof(char));
	strcpy(Que->queue,temp);
	Que->num -= 1;
	return ch;
};
void init(Queue *Que){
	memset(Que->queue,0,5);
	Que->num = 0;
	Que->append = attach;
	Que->get = get;
};
Queue queue1,queue2;

//#include"queue.h"
// mutex1用于队列1的互斥信号量
// mutex2用于队列2的互斥信号量
// space1,space2用于记录队列1,2剩余空间的数目的计数信号量
// num1，num2用于记录队列1,2的资源数目的计数信号量
/*semaphore declared in here! */
sem_t mutex1,mutex2,space1,space2,num1,num2;
/*relative functions*/
#define semWait(x)		sem_wait(&x) //P function
#define semSignal(x)	sem_post(&x)//V function
#define check(x) 		if( (x) != 0 )	{printf("\033[31merror at[%s],[func %s],[line %d],ret val is %d!\033[37m\n"\
						,__FILE__,__FUNCTION__,__LINE__,x);return -1;}
#define LOG(x) 			printf("\033[32m\t\t\t\t\t\t\t\t%c running id:[%ld]\033[31m\n\033[37m"\
								 + ((x=='A')?13:(x=='B')?9:0),x,pthread_self())
#define toUpper(x)		(char)( (x>=97)?(x<=122)?(x - 32):x:x )
static inline void produce(char *ch){
	scanf("%s",ch);
}
/*take implemention*/
static inline char take(Queue *que){
	printf("\t\t\t\ttake [%c] from Q1[%s]\n",*que->queue,que->queue);
	char ch = que->get(que);
	return ch;
}
/*append implemention*/
static inline void append(Queue *que,char ch,char label){
	que->append(que,ch);
	printf("\t\t\t\tappend [%c] to Q%d [%s]\n" + ((label=='A')?4:0),ch,((label=='A')?1:2),que->queue);
}
/*consume implemention*/
static inline void consume(Queue *que){
	printf("\t\t\t\t\t\t\t\t\033[32mget [\033[33m%c\033[32m] from Q2 [%s]\033[37m\n",*que->queue,que->queue);
	char ch = que->get(que);
}	

void *threadA(void* param){
	char buffer[5];//
	char* ch = buffer;//if use malloc, remember to free
	int i;	
	while(1){/*process for multli input*/
		LOG('A');
		produce(ch);
		if( *ch == 'q' ){
			printf("input char 'q',A exited, which will cause B and C exit!\n");
			pthread_exit(NULL);
		}else{
			i = strlen(ch);
			printf("A input : [%s,%d]\n",ch,i);
		};
		while(i>0){
			semWait(space1);
			semWait(mutex1);
			/*append function*/
			append(&queue1,*(ch+i-1),'A');
			/*-----*/
			semSignal(mutex1);
			semSignal(num1);
			i --;
		};
	}
};
void *threadB(void* param){
	char ch ;
	while(1){
		LOG('B');
		/*take 1*/
		semWait(num1);
		semWait(mutex1);
		/*take function*/
		ch = take(&queue1);
		/*-------------*/
		semSignal(mutex1);
		semSignal(space1);
		/*produce 2*/
		semWait(space2);
		semWait(mutex2);
		/*append function*/
		append(&queue2,toUpper(ch),'B');
		/*-------------*/
		semSignal(mutex2);
		semSignal(num2);
	}
};
void *threadC(void* param){
	while(1){
		LOG('C');
		semWait(num2);
		semWait(mutex2);
		/*consume function*/
		consume(&queue2);
		/*-------------*/
		semSignal(mutex2);
		semSignal(space2);
	}
};


int main(int argc,char *argv[]){
	/*Queue define*/
	init(&queue1);
	init(&queue2);
	/*semaphore init*/
	check( sem_init( &mutex1,0,1) );
	check( sem_init( &mutex2,0,1) );
	check( sem_init( &space1,0,5) );
	check( sem_init( &space2,0,5) );
	check( sem_init( &num1  ,0,0) );
	check( sem_init( &num2  ,0,0) );		
	/*thread init*/
	pthread_t A,B,C;
	check( pthread_create(&A,NULL,threadA,NULL) );
	check( pthread_create(&B,NULL,threadB,NULL) );
	check( pthread_create(&C,NULL,threadC,NULL) );
	/*wait thread exit*/
	/*if A exited, then B will exit, then C will exit*/
	check( pthread_join(A,NULL) );
	sleep(1);	
	check( pthread_cancel(B) );
	check( pthread_cancel(C) );
	/*semaphore destory*/
	check( sem_destroy( &mutex1 ) );
	check( sem_destroy( &mutex2 ) );
	check( sem_destroy( &space1 ) );
	check( sem_destroy( &space2 ) );
	check( sem_destroy( &num1   ) );
	check( sem_destroy( &num2   ) );
	return 0;
};


