
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>

/* Remember to initilize the queue before using it */
void initialize_queue(struct pqueue_t *q)
{
	q->head = q->tail = NULL;
	pthread_mutex_init(&q->lock, NULL);
}

/* Return non-zero if the queue is empty */
int empty(struct pqueue_t *q)
{
	return (q->head == NULL);
}

/* Get PCB of a process from the queue (q).
 * Return NULL if the queue is empty */
struct pcb_t *de_queue(struct pqueue_t *q)
{
	struct pcb_t *proc = NULL;
	// TODO: return q->head->data and remember to update the queue's head
	// and tail if necessary. Remember to use 'lock' to avoid race
	// condition

	// YOUR CODE HERE
	pthread_mutex_lock(&q->lock);
	if (!empty(q))
	{
		proc = q->head->data;
		struct qitem_t *tmp = q->head;

		if (q->head->next == NULL)
			q->tail = NULL;
		q->head = q->head->next;

		free(tmp);
	}

	pthread_mutex_unlock(&q->lock);

	return proc;
}

/* Put PCB of a process to the queue. */
void en_queue(struct pqueue_t *q, struct pcb_t *proc)
{
	// TODO: Update q->tail.
	// Remember to use 'lock' to avoid race condition

	// YOUR CODE HERE
	pthread_mutex_lock(&q->lock);
	struct qitem_t *new_item = malloc(sizeof(struct qitem_t));
	new_item->data = proc;
	new_item->next = NULL;
	if (empty(q))
	{
		q->head = q->tail = new_item;
	}
	else
	{
		q->tail->next = new_item;
		q->tail = new_item;
	}

	pthread_mutex_unlock(&q->lock);
}
void en_priority_queue(struct pqueue_t *q, struct pcb_t *proc)
{
	pthread_mutex_lock(&q->lock);
	struct qitem_t *new_item = malloc(sizeof(struct qitem_t));
	new_item->data = proc;
	new_item->next = NULL;

	if (empty(q))
	{
		q->head = q->tail = new_item;
	}
	else
	{
		if (q->head->data->priority >= new_item->data->priority)
		{
			new_item->next = q->head;
			q->head = new_item;
			pthread_mutex_unlock(&q->lock);
			return;
		}
		// add middle
		struct qitem_t *tmp = q->head;
		while (tmp->next)
		{
			if (tmp->next->data->priority >= new_item->data->priority)
			{
				new_item->next = tmp->next;
				tmp->next = new_item;
				pthread_mutex_unlock(&q->lock);
				return;
			}
			tmp = tmp->next;
		}
		en_queue(q, proc);
	}

	pthread_mutex_unlock(&q->lock);
}
