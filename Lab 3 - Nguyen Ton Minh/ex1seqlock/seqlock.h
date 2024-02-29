#ifndef PTHREAD_H
#include <stdio.h>
#include <pthread.h>
#endif

typedef struct pthread_seqlock { 
   pthread_mutex_t lock;
   unsigned int sequence;
   } pthread_seqlock_t;


static inline void pthread_seqlock_init(pthread_seqlock_t *rw)
{
   pthread_mutex_init(&rw->lock, NULL);
   rw->sequence = 0;
   printf( "pthread_seqlock_init") ;
}

static inline void pthread_seqlock_wrlock(pthread_seqlock_t *rw)
{
   pthread_mutex_lock(&rw->lock);
   rw->sequence++;
   printf( "seqlock_wrlock: ");
}

static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *rw)
{
   rw->sequence++;
   pthread_mutex_unlock(&rw->lock);
   printf("seqlock_wrunlock: " ) ;
}

static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *rw)
{
   unsigned int seq;
   pthread_mutex_lock(&rw->lock);
   seq = rw->sequence;
   printf("seqlock_rdlock: ");
   if (seq & 1) {
      pthread_mutex_unlock(&rw->lock);
      printf("seqlock_rdlock: ") ;
      return 0;
   }
   return seq;
}



static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *rw)
{
   pthread_mutex_unlock(&rw->lock);
   printf("seqlock_rdunlock: ");
   return 0;
}

