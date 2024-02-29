#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 5

enum {THINKING,HUNGRY,EATING} state[5];

pthread_mutex_t mtx;
pthread_cond_t self[N];

void *philosopher(void*);
void eat(int);
void think(int);
int main()
{
   int i, a[N];
   pthread_t tid[N];

   //begin protection mechanism
   
   pthread_mutex_init(&mtx, NULL);
   for (i = 0; i < N; i++)
	   pthread_cond_init(&self[i], NULL);

   // end protection mechanism
   for (i = 0; i < N; i++) {
      state[i] = THINKING;
   }
   for (i = 0; i < 5; i++)
   {
      a[i] = i;
      pthread_create(&tid[i], NULL, philosopher, (void*) &a[i]);
   }

   for (i = 0; i < 5; i++)
      pthread_join(tid[i], NULL);
}

void *philosopher(void *num)
{
   int phil = *(int*) num;
   printf("Philosopher %d has entered room\n", phil);

   while (1)
   {
      state[phil] = HUNGRY;
      if (state[phil + 1] %5 != EATING && state[phil + 4] %5 != EATING && state[phil] %5 == HUNGRY)
      {

         printf("Philosopher %d takes fork %d and %d up\n",
                phil, phil, (phil + 1) % N);
         eat(phil);
         state[phil] = EATING;
         pthread_cond_signal(&self[phil]);
      }
      else{
         pthread_cond_wait(&self[phil], &mtx);
      }
      sleep(2);
  
       printf("Philosopher %d puts fork %d and %d down\n",
	          phil, (phil + 1) % N, phil);
      think(phil);
      state[phil] = THINKING;
	  sleep(1);
   }
}

void eat(int phil)
{
   printf("Philosopher %d is eating\n", phil);
}

void think(int phil)
{
   printf("Philosopher %d is thinking\n", phil);
}