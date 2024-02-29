#include <stdlib.h>
#include <stdio.h>

int arr_size = 0;

int main(void){
	printf("Array size= ");
	scanf("%d",&arr_size);
	
	int sum =0;
	for (int i=0; i<arr_size; i++) sum+=(i+1);
	printf("sum=%d\n",sum); 
}
