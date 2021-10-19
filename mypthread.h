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

typedef struct threadControlBlock {
	/* add important states in a thread control block */
	int thread_id;				// thread Id
	thread_status status;		// thread status
	ucontext_t * context;		// thread context
	void * return_ptr;			// pointer to return value

	// YOUR CODE HERE
} tcb;

tcb* createNewTCB(){
	tcb* temp = (tcb*) malloc(sizeof(tcb));
	temp->thread_id = next_thread_id;
	next_thread_id++;
	temp->status= READY;
	temp->context = (ucontext_t*) malloc(sizeof(ucontext_t));
	return temp;
}

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

typedef enum _boolean{TRUE = 1, FALSE = 0} bool;

typedef enum _thread_status{READY, RUNNING, WAITING, DELAYED, BLOCKED, FINISHED } thread_status;

typedef uint mypthread_t;

void initqueue(int threadid, tcb* tb);
static void schedule();

static ucontext_t schedule_ctx, main_ctx, no_exit_ctx;
static tcb main_tcb; 
char schedule_stack[32000], no_exit_stack[32000];
bool schedule_created = FALSE;
bool schedule_space_lock = FALSE;

void create_schedule_ctx(){
	schedule_created++;
	schedule_ctx.uc_stack.ss_sp = &schedule_stack;
	schedule_ctx.uc_stack.ss_size = 32000;
	schedule_ctx.uc_link = &schedule_ctx;
	makecontext(&schedule_ctx, schedule ,0);
}

void create_no_exit_ctx(){
	no_exit_ctx.uc_stack.ss_sp = &no_exit_stack;
	no_exit_ctx.uc_stack.ss_size = 32000;
	no_exit_ctx.uc_link = &schedule_ctx;
	makecontext(&no_exit_ctx, mypthread_no_exit,0);
}
void mypthread_no_exit();

static poolnode * front;
/*declaration and functions for the Queue */

static Queue* runQueue;

typedef struct _QNode{
	tcb * tcb_ptr;
	struct _QNode* next;
} QNode;

typedef struct _Queue{
	QNode *front, *rear;
}Queue;

QNode* newNode(tcb* new_ptr){
	QNode* temp =(QNode*)malloc(sizeof(QNode));
	temp->tcb_ptr;
	temp->next = NULL;
	return temp;
}

Queue* createQueue(){
	Queue* q = (Queue*) malloc(sizeof(Queue));
	q->front = NULL;
	q->rear = NULL;
	return q;
}

void enQueue(Queue* q, tcb * new_ptr){
	QNode* temp = newNode(new_ptr);
	if(q->rear == NULL){
		q->front = temp;
		q->rear = temp;
		return;
	}

	(q->rear)->next = temp;
	q->rear = temp;
	if (q->front == NULL)
        q->rear = NULL;
}

void deQueue(Queue * q){
	if (q->front == NULL){
		return;
	}
	QNode* temp = q->front;
	q->front = q->front->next;
	if (q->front == NULL){
        q->rear = NULL;
	}
	free(temp);
}
/* Function Declarations: */
QNode* newNode(tcb* new_ptr);

Queue* createQueue();

void deQueue(Queue * q);

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
