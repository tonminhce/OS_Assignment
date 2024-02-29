#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int MAX_COUNT = 1e9;
static int count = 0;
pthread_mutex_t lock;
void *f_count(void *sid) {
    // Locks a mutex object, which identifies a mutex.
    // If the mutex is already locked by another thread, the thread waits for the mutex to become available.
    // The thread that has locked a mutex becomes its current owner and remains the owner until the same thread has unlocked it
    pthread_mutex_lock(&lock);

    int i;
    for (i = 0; i < MAX_COUNT; i++) {
        count = count + 1;
    }

    printf("Thread %s: holding %d \n", (char *)sid, count);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    // Creates a mutex, referenced by mutex, with attributes specified by attr. If attr is NULL,
    // the default mutex attribute (NONRECURSIVE) is used.
    // If successful == 0, else 1
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n Mutex Init Has Failed");
        return 1;
    }

    /* Create independent threads each of which will execute function */
    pthread_create(&thread1, NULL, &f_count, "1");
    pthread_create(&thread2, NULL, &f_count, "2");

    // Wait for thread th1 finish
    pthread_join(thread1, NULL);

    // Wait for thread th1 finish
    pthread_join(thread2, NULL);
    // Deletes a mutex object, which identifies a mutex. Mutexes are used to protect shared resources.
    // Mutex is set to an invalid value, but can be reinitialized using pthread_mutex_init().
    pthread_mutex_destroy(&lock);
    return 0;
}