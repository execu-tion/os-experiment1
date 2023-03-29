/*queue*/
#include"queue.h"
#include<malloc.h>
#include<stdlib.h>
queue* initial(void){
	queue *qu = (queue *)malloc(sizeof(queue));
	if(qu == NULL) return NULL;
	qu -> length = 0;
	qu -> head = NULL;
	qu -> tail = NULL;
	return qu;
};
int push(queue* queue,char ch){
	if( queue == NULL ) return -1;
	node *nod = (node *)malloc(sizeof(node));		
	if( queue-> length == 0){
		queue -> head = nod;
		queue -> head -> prev = NULL;
		queue -> head -> next = NULL;
		queue -> length = 1;
		queue -> head -> ch = ch;
		queue -> tail = queue -> head;
	}else{
		queue -> length += 1;
		nod -> next = NULL;
		nod -> ch = ch;
		nod -> prev = queue -> tail;
		nod -> prev -> next = nod;
		queue -> tail = nod;
	}
	return 0;
}
char pop(queue *queue){
	char ch ;
	if( queue == NULL) return -1;
	if( queue -> length == 0) return -1;
	if( queue -> length == 1 ){
		queue -> length -= 1;
		ch = queue -> head -> ch;
		free( queue -> head);
		queue->head = NULL;
		free( queue );
		queue = NULL;
	}else{
		queue -> length -= 1;
		node *nod = queue -> tail;
		queue -> tail = queue -> tail -> prev;
		queue -> tail -> next = NULL;
		ch = nod ->ch;
		free(nod);
		nod = NULL;
	}

	return ch;
}





