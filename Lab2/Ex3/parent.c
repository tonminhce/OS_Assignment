#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int pipfd1[2], pipfd2[2];
#define MESSAGE_SIZE 20

char send_Parent[MESSAGE_SIZE] = "Message to Parent";
char send_Child[MESSAGE_SIZE] = "Message to Children";

char read_Parent[MESSAGE_SIZE];
char read_Child[MESSAGE_SIZE];

void INIT(void){
	if (pipe(pipfd1)<0 || pipe(pipfd2)<0){
		perror("pipe");
		exit(EXIT_FAILURE);
	} 
}

void WRITE_TO_PARENT(void){
	write(pipfd1[1], send_Parent, MESSAGE_SIZE);
	printf("Child send message to parent: %s\n",send_Parent);
}

void READ_FROM_PARENT(void){
	read (pipfd2[0], read_Child, MESSAGE_SIZE);
	printf("Child receive message from parent: %s\n",read_Child);	
}

void WRITE_TO_CHILD(void){
	write(pipfd2[1], send_Child, MESSAGE_SIZE);
	printf("Parent send message to Child from pipe:%s\n",send_Child);
}

void READ_FROM_CHILD(void){
	read (pipfd1[0], read_Parent, MESSAGE_SIZE);
	printf("Parent receive message from child: %s\n",read_Parent);
}
int main(int argc, char* argv[]){
	INIT();
	pid_t pid;
	pid = fork();
	
	if (pid == 0){
		while(1){
			sleep(rand()%2 + 1);
			WRITE_TO_PARENT();
			READ_FROM_PARENT();
		}
	}
	else {
		while(1){
			sleep(rand()%2+1);
			READ_FROM_CHILD();
			WRITE_TO_CHILD();
		}	
	}
}
