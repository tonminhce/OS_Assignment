#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>



int main(void){
	FILE *f1 = fopen("movie-100k_1.txt","r");
	FILE *f2 = fopen("movie-100k_2.txt","r");
	char *line = NULL;
	char *token = NULL;
	size_t length = 0;
	int state_a, state_b;
	pid_t child_a, child_b;
	int (*table)[100];
	int shmid;
	shmid = shmget(IPC_PRIVATE, sizeof(int[100][100]), 0644|IPC_CREAT);
	table = shmat(shmid, NULL,0);
	child_a = fork();
	if (child_a==0){
		table = shmat(shmid, NULL,0);
		while(getline(&line,&length,f1)!=-1){
			int index = 0;
			int user = 0;
			int movie = 0;
			int rate = 0;
			getline(&line,&length,f1);
			token = strtok(line,"\t");
			while (token!=NULL){
				index ++;
				if (index==1) user = atoi(token);
				if (index==2) movie = atoi(token);
				if (index==3) rate = atoi(token);
				token = strtok(NULL,"\t");
			}
			table[movie][user] = rate;
		}
		fclose(f1);
		exit(1);
	}else{
		child_b = fork();
		if (child_b==0){
			table = shmat(shmid, NULL,0);
			while(getline(&line,&length,f2)!=-1){
				int index = 0;
				int user = 0;
				int movie = 0;
				int rate = 0;
				getline(&line,&length,f2);
				token = strtok(line,"\t");
				while (token!=NULL){
					index ++;
					if (index==1) user = atoi(token);
					if (index==2) movie = atoi(token);
					if (index==3) rate = atoi(token);
					token = strtok(NULL,"\t");
				}
				table[movie][user] = rate;
			}
			fclose(f2);
			exit(2);
		} else{
			pid_t a = waitpid(child_a, &state_a, 0);
			pid_t b = waitpid(child_b, &state_b, 0);
			for (int row =0; row<100; row++){
				float temp = 0;
				int count =0;
				for(int col=0; col<100;col++){
					if (table[row][col]!=0){
						count++;
						temp = temp + table[row][col];
					}
				}
				if (count!=0){
					temp/=count;
					printf("RATE of %d:%.5f\n",row,temp);
				}
			}
		}
	}
}
