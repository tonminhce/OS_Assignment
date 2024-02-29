#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int thread_num = 0;
int arr_size = 0;

typedef struct {
	int *data;
	int id;
} thread_arr;

void* subsum(void* pointer){
	thread_arr* ptr = (thread_arr*)pointer;
	int id = ptr->id;
	int step = arr_size/thread_num;
	int des = arr_size;
	int* thread_sum = (int*) calloc(1,sizeof(int));
	
	for (int index=0; index<thread_num; index++){
		if (id==index){
			if (step*(id+1)<des && id!=thread_num-1) des = step*(id+1);
			else des = arr_size;
			 
			for (int i = id*step; i<des; i++){
				thread_sum[0] = thread_sum[0] + ptr->data[i];
			}
		}
	}
	pthread_exit(thread_sum);
}

int main(void){
	
	printf("Thread number:");
	scanf("%d",&thread_num);
	
	printf("Array size:");
	scanf("%d",&arr_size);

	int* arr = (int*) calloc(arr_size, sizeof(int));
	thread_arr thread_array[thread_num]; 
	
	for (int i= 0; i<arr_size; i++)
		arr[i] = i + 1;
	for (int i= 0; i<thread_num; i++){
		thread_array[i].data = arr;
		thread_array[i].id = i; 
	}
	
	pthread_t tid[thread_num];
	for (int i=0; i<thread_num; i++)
		pthread_create(&tid[i], NULL, subsum, &thread_array[i]);
	
	int* sub_sum;
	int sum=0;
	for (int i=0; i<thread_num; i++){
		pthread_join(tid[i],(void**)&sub_sum);
		sum+=(*sub_sum);
	}
	printf("sum=%d\n",sum);
}
