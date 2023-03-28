#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<sys/types.h>
#include<time.h>
#include<malloc.h>
#include<sys/shm.h>
#include<sys/ipc.h>

#define SIZE 10

int main(int argc,char *argv[]){
	pid_t pid;
	/*unshared memery*/
	char *unshared_value = "unshared value";
	/*shared memeory*/
	int statloc,k=12;
	int shmid;
	key_t key;
	char *val;
	if( (key = ftok("./",k)) == -1){return -1;};
	if( (shmid = shmget(key,SIZE,IPC_CREAT|0666)) <0 ){return -1;};
	if( (val = shmat(shmid,0,0)) < 0){return -1;};
	strcpy(val,"shared value");
	/*fork */
	pid = fork();
	/*in parent process*/
	if( pid > 0 ){
		sleep( ( (argv[1]==NULL)?0:*argv[1] - 48 ));
		printf("in parent process\n");
		printf("my pid is :\t%d\n",getpid());
		printf("my child pid is :\t%d\n",pid);
		printf("shared val is :<\033[32m%s\033[37m>\nunshared val is :<\033[32m%s\033[37m>[\033[32m%p\033[37m]\n",val,unshared_value,unshared_value);
		printf("parent process blocked\n");
		pid_t pid_e = wait(&statloc);
		printf("shared val is :<\033[32m%s\033[37m>\nunshared val is :<\033[32m%s\033[37m>[\033[32m%p\033[37m]\n",val,unshared_value,unshared_value);
		printf("child exit status is %d\n",WEXITSTATUS(statloc));
		printf("parent process exit,pid is %d\n",getpid());
		
		shmdt(val);
		shmctl(shmid,IPC_RMID,0);
	}else if( pid == 0 ){
		/*in child process*/
		printf("\t\t\t\t\tin child process\n");
		printf("\t\t\t\t\tmy pid is :\t%d\n",getpid());
		printf("\t\t\t\t\tmy parent pid is\t%d\n",getppid());
		printf("\t\t\t\t\tshared val is :<\033[32m%s\033[37m>\n\t\t\t\t\tunshared value is:<\033[32m%s\033[37m>[\033[32m%p\033[37m]\n",val,unshared_value,unshared_value);	
	
		if( execl("./demo","demo",&k,unshared_value,NULL) != 0 ){
			printf("\t\t\t\t\tcall function failed\n");
		}else{
			printf("\t\t\t\t\tcall function return\n");
		};
	}
	else if(pid < 0){
		return -1;
	}else{
	};
	return 0;
}
