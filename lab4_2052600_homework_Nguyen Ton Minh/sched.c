
#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_UNIT	100 // In microsecond

static struct pqueue_t in_queue; // Queue for incomming processes
static struct pqueue_t ready_queue; // Queue for ready processes
int load_done = 1;
int cpu_execute = 0;

static int timeslot; 	// The maximum amount of time a process is allowed
			// to be run on CPU before being swapped outs

// Emulate the CPU
void * cpu(void * arg) {
	int timestamp = 0;
	while(1)
	{
		if(load_done == 0) break;
	}
	while (!load_done || !empty(&ready_queue)) {
		/* Pickup the first process from the queue */
		struct pcb_t * proc = de_queue(&ready_queue);
		if (proc == NULL) {
			/* If there is no process in the queue then we
			 * wait until the next time slice */
			timestamp++;
			usleep(TIME_UNIT);
		}else{
			/* Execute the process */
			int start = timestamp; 	// Save timestamp
			int id = proc->pid;	// and PID for tracking

			//INITIALIZE EXECUTE TIME OF EACH P IS 1MS
			int exec_time = 1;
			// TODO: Calculate exec_time from process's PCB
			// LET THIS JOB EXECUTE 1MS
			proc -> burst_time -= exec_time;
			usleep(exec_time * TIME_UNIT);
			
			/* Update the timestamp */
			timestamp += exec_time;

			// TODO: Check if the process has terminated (i.e. its
			// burst time is zero. If so, free its PCB. Otherwise,
			// put its PCB back to the queue.
			if(proc->burst_time == 0) free(proc);
			else{
				proc -> priority -= 1;
				en_priority_queue(&ready_queue,proc);
			}
			// YOUR CODE HERE
			
			/* Track runtime status */
			printf("%2d-%2d: Execute %d\n", start, timestamp, id);
		}
		cpu_execute = 1;
	}
}

// Emulate the loader
void * loader(void * arg) {
	int timestamp = 0;
	/* Keep loading new process until the in_queue is empty*/
	while (!empty(&in_queue)) {
		struct pcb_t * proc = de_queue(&in_queue);
		/* Loader sleeps until the next process available */
		int wastetime = proc->arrival_time - timestamp;
		usleep(wastetime * TIME_UNIT);
		/* Update timestamp and put the new process to ready queue */
		timestamp += wastetime;
		
		en_priority_queue(&ready_queue, proc);
		load_done = 0;
	}
	/* We have no process to load */
	while(1)
	{
		if(cpu_execute == 1)
		{
			load_done = 1;
			break;
		}
	}
}

/* Read the list of process to be executed from stdin */
void load_task() {
	int num_proc = 0;
	scanf("%d %d\n", &timeslot, &num_proc);
	int i;
	for (i = 0; i < num_proc; i++) {
		struct pcb_t * proc =
			(struct pcb_t *)malloc(sizeof(struct pcb_t));
		scanf("%d %d %d\n", &proc->arrival_time, &proc->burst_time, &proc->priority);
		proc->pid = i;
		en_queue(&in_queue, proc);
	}
}

int main() {
	pthread_t cpu_id;	// CPU ID
	pthread_t loader_id;	// LOADER ID
	pthread_mutex_t mtx;
	pthread_mutex_init(&mtx,NULL);
	/* Initialize queues */
	initialize_queue(&in_queue);
	initialize_queue(&ready_queue);

	/* Read a list of jobs to be run */
	pthread_mutex_lock(&mtx);
	load_task();
	pthread_mutex_unlock(&mtx);
	/* Start loader */
	pthread_create(&loader_id, NULL, loader, NULL);
	/* Start cpu */
	pthread_create(&cpu_id, NULL, cpu, NULL);
	

	/* Wait for cpu and loader */
	pthread_join(cpu_id, NULL);
	pthread_join(loader_id, NULL);

	pthread_exit(NULL);

}


