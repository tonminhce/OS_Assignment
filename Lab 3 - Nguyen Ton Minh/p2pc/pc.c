#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 1
#define THREADS 1           // 1 producer and 1 consumer
#define LOOPS 5 * MAX_ITEMS // variable

// Initiate shared buffer
int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;

/*TODO: Fill in the synchronization stuff */
sem_t empty;
sem_t full;
pthread_mutex_t mutex;
void put(int value); // put data into buffer
int get();           // get data from buffer

void *producer(void *arg) {
    int i;

    int tid = (int)arg;
    for (i = 0; i < LOOPS; i++) {
        /*TODO: Fill in the synchronization stuff */
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        put(i); // line P2
        printf("Producer %d put data %d\n", tid, i);
        sleep(1);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        /*TODO: Fill in the synchronization stuff */
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int i, tmp = 0;
    int tid = (int)arg;
    while (tmp != -1 && i < LOOPS) {
        /*TODO: Fill in the synchronization stuff */
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        tmp = get(); // line C2
        printf("Consumer %d get data %d\n", tid, tmp);
        sleep(1);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        i++;
        /*TODO: Fill in the synchronization stuff */
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    int i, j;
    int tid[THREADS];
    pthread_t producers[THREADS];
    pthread_t consumers[THREADS];

    /*TODO: Fill in the synchronization stuff */
    sem_init(&empty, 0, MAX_ITEMS);
    sem_init(&full, 0, 0);
    for (i = 0; i < THREADS; i++) {
        tid[i] = i;
        // Create producer thread
        pthread_create(&producers[i], NULL, producer, (void *)tid[i]);

        // Create consumer thread
        pthread_create(&consumers[i], NULL, consumer, (void *)tid[i]);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    /*TODO: Fill in the synchronization stuff */
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}

void put(int value) {
    // TODO
    buffer[fill] = value;          // line f1
    fill = (fill + 1) % MAX_ITEMS; // line f2
}

int get() {
    // TODO
    int tmp = buffer[use];       // line g1
    use = (use + 1) % MAX_ITEMS; // line g2
    return tmp;
}