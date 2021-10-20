// File:	mypthread_t.h

// List all group member's name:
// username of iLab:
// iLab Server:

#ifndef MYTHREAD_T_H
#define MYTHREAD_T_H

#define _GNU_SOURCE

/* To use Linux pthread Library in Benchmark, you have to comment the USE_MYTHREAD macro */
#define USE_MYTHREAD 1

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <stdatomic.h>
typedef enum _boolean{TRUE = 1, FALSE = 0} bool;

typedef enum _thread_status{
	READY, RUNNING, WAITING, DELAYED, BLOCKED, FINISHED 
	} thread_status;

static int next_thread_id = 0002;

typedef struct threadControlBlock {
	/* add important states in a thread control block */
	int thread_id;				// thread Id
	thread_status status;		// thread status
	ucontext_t * context;		// thread context
	void * return_ptr;			// pointer to return value

	// YOUR CODE HERE
} tcb;

tcb* createNewTCB();

/* mutex struct definition */
typedef struct mypthread_mutex_t {
	/* add something here */

	// YOUR CODE HERE
} mypthread_mutex_t;

/* define your data structures here: */
// Feel free to add your own auxiliary data structures (linked list or queue etc...)

// YOUR CODE HERE
typedef struct _poolnode{
	struct _poolnode* previous;
	struct _poolnode* next;
	tcb * currentTcb;
	int threadid;
}	poolnode;

/* new declarations */

#define handle_error(msg) \ 
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef uint mypthread_t;

void initqueue(int threadid, tcb* tb);

void create_schedule_ctx();

void mypthread_no_exit();

static poolnode * front;
/*declaration and functions for the Queue */
static tcb* runningThread = NULL;


typedef struct _QNode{
	tcb * tcb_ptr;
	struct _QNode* next;
} QNode;

typedef struct _Queue{
	QNode *front, *rear;
}Queue;

static Queue runQueue = {.front = NULL, .rear = NULL};

/* Function Declarations: */
QNode* newNode(tcb* new_ptr);

Queue* createQueue();

tcb* deQueue(Queue * q);

void enQueue(Queue* q, tcb * new_ptr);

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
