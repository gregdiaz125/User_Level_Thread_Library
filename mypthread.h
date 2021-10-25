// File:	mypthread_t.h

// List all group member's name:
// username of iLab:
// iLab Server:

#ifndef MYTHREAD_T_H
#define MYTHREAD_T_H

#define _GNU_SOURCE
#define MLFG

#define POlICY  "sched_fcfs()"


/* To use Linux pthread Library in Benchmark, you have to comment the USE_MYTHREAD macro */
#define USE_MYTHREAD 1
#define THREAD_QUANTUM 5000
/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <stdatomic.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>


typedef enum _boolean{TRUE = 1, FALSE = 0} bool;

#define handle_error(msg)\
	do { perror(msg); exit(EXIT_FAILURE); } while (0)


typedef uint mypthread_t;

typedef enum _thread_status{
	READY, RUNNING, WAITING, FINISHED
	} thread_status;

static int next_thread_id = 0002;

typedef struct threadControlBlock {
	/* add important states in a thread control block */
	int thread_id;				// thread Id
	thread_status status;		// thread status
	ucontext_t* context;		// thread context
	uint time_elapsed;
	void * return_ptr;
	void ** join_ptr;			// pointer to return value
	struct threadControlBlock* 	// Thread that has called joined and
				waiting_thread;	// is waiting for this thead to finish	 
								
	// YOUR CODE HERE
} tcb;

static ucontext_t schedule_ctx, main_ctx, exit_ctx;

static tcb main_tcb = {	.thread_id = 1, .status= RUNNING, .time_elapsed = 0,
						.join_ptr =NULL ,.return_ptr = NULL, .context = &main_ctx}; 

static char schedule_stack[32000], no_exit_stack[32000];

static bool schedule_created = FALSE;

static bool schedule_space_lock = FALSE;

static tcb* running_thread = &main_tcb;


tcb* createnew_tcb();

void create_schedule_ctx();

void free_thread(tcb *exit_ptr);

void mypthread_no_exit();

void create_exit_ctx();


/* mutex struct definition */

typedef struct mypthread_mutex_t {
	atomic_flag lock;
} mypthread_mutex_t;

atomic_flag s_lock ;
atomic_flag e_lock;

void set_timer();

void stop_timer();

void timer_interrupt(int sig);

void init_sigact();


/* new declarations */
/*declaration and functions for the Queue */


typedef struct _QNode{
	tcb * tcb_ptr;
	struct _QNode* next;
} QNode;

typedef struct _Queue{
	QNode *front, *rear;
}Queue;

static QNode mainNode = {.tcb_ptr = &main_tcb , .next = NULL};

static Queue runQueue = {.front = NULL, .rear = NULL};


static Queue thread_pool= {	.front = &mainNode, 
							.rear = &mainNode};


/* Function Declarations: */
static void schedule();

tcb * sched_stcf();

tcb * sched_fcfs();

QNode* newNode(tcb* new_ptr);

Queue* createQueue();

tcb* deQueue(Queue * q);

void enQueue(Queue* q, tcb * new_ptr);

tcb* searchQueue(Queue* q, int id);

void printQueue(Queue * q);

void removefromQueue(Queue *q, tcb* target);
/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void
    *(*function)(void*), void * arg);

/* give CPU pocession to other user level threads voluntarily */
int mypthread_yield();

/* terminate a thread */
void mypthread_exit(void *value_ptr);

/* wait for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr);

/* initial the mutex lock */
int mypthread_mutex_init(mypthread_mutex_t *mutex, const pthread_mutexattr_t
    *mutexattr);

/* aquire the mutex lock */
int mypthread_mutex_lock(mypthread_mutex_t *mutex);

/* release the mutex lock */
int mypthread_mutex_unlock(mypthread_mutex_t *mutex);

/* destroy the mutex */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex);

#ifdef USE_MYTHREAD
#define pthread_t mypthread_t
#define pthread_mutex_t mypthread_mutex_t
#define pthread_create mypthread_create
#define pthread_exit mypthread_exit
#define pthread_join mypthread_join
#define pthread_mutex_init mypthread_mutex_init
#define pthread_mutex_lock mypthread_mutex_lock
#define pthread_mutex_unlock mypthread_mutex_unlock
#define pthread_mutex_destroy mypthread_mutex_destroy
#endif

#endif
