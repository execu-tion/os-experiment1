#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<sys/types.h>
#include<time.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<string.h>		

int main(int argc,char* argv[]){
	int k = *argv[1];
	key_t key;
	int shmid;
	char *shmaddr;

	if( argc <= 1 ) return -1;

	printf("\t\t\t\t\t---> function '%s' called!\tmy pid is :%d\tmy parent pid is :%d\n",argv[0],getpid(),getppid());
	
	key = ftok("./",k);
	if( (shmid = shmget(key,10,0)) <0 ) return -1;
	shmaddr = shmat(shmid,0,0);
	
	if(argc == 3){
	printf("\t\t\t\t\t---> shared val is :<\033[32m%s\033[37m>\n\t\t\t\t\t---> unshared value is:<\033[32m%s\033[37m>[\033[32m%p\033[37m]\n",shmaddr,argv[2],argv[2]);
	argv[2] = "demo";
	printf("\t\t\t\t\t---> unshared val change to :\t\033[32m%s\033[37m\n",argv[2]);
	}
	strcpy(shmaddr,"demo");
	printf("\t\t\t\t\t---> shared val change to :\t\033[32m%s\033[37m\n",shmaddr);	
	printf("\t\t\t\t\t---> function exit!\n");

	shmdt(shmaddr);
	shmctl(shmid,IPC_RMID,0);
	
	return 0;
}

int test(int argc,char *argv[]){
	//printf("%d\n",argc);
	//printf("%s\n",argv[1]);
	int a = 0;
	printf("%d\n",a);
	char* b[2];
	char c ='a';
	b[0] = &c;
	b[1] = (char *)(&a);
	printf("+%p\n",&a);
//	increase(2,b);
	printf("+%d\n",a);
	return 0;
}

