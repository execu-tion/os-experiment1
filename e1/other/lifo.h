typedef struct node{
        struct node *prev;
        char ch;
        struct node *next;
}node;
typedef struct queue{
        node *head;
        node *tail;
        int length;
}queue;

queue* initial(void);
int push(queue* queue,char ch);
char pop(queue* queue);
