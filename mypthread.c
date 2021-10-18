// File:	mypthread.c

// List all group member's name: Gregory Diaz
// username of iLab: gldd55
// iLab Server:

#include "mypthread.h"
#include <ucontext.h>
// INITAILIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE
int next_thread_id = 0002;

static ucontext_t uctx_main;
static ucontext_t schedule_context;
char schedule_stack[32000];
static queueNode * front;
int schedule_created = 0;

void initRunqueue(int id, tcb* tb){
	front = (queueNode*) malloc(sizeof(queueNode));
	front->previous = front;
	front->next = front;
	front->threadid = id;
}

void push(int id,tcb* tb ){
	if(front == NULL){ handle_error("queueNULL");}
	queueNode *newNode = (queueNode*) malloc(sizeof(queueNode));
	newNode->currentTcb = tb;
	newNode->threadid = id;
	newNode->next = front;
	newNode->previous = front->previous;
	front->previous->next = newNode;
	front->previous = newNode;
}

void pop(){
	front->previous->next= front->next;
	front->next->previous = front->previous;
	queueNode *temp = front;
	free(front);
	front = temp;
}

/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr,
                      void *(*function)(void*), void * arg) {
       // create Thread Control Block
       // create and initialize the context of this thread
       // allocate space of stack for this thread to run
       // after everything is all set, push this thread int
       // YOUR CODE HERE
	tcb* newtcb = malloc(sizeof(tcb));
	newtcb->thread_id = next_thread_id;
	next_thread_id++;
	newtcb->priority = 0;
	newtcb->status = READY;
	newtcb->context = (ucontext_t*) malloc(sizeof(ucontext_t));
	if(getcontext(newtcb->context) == -1){ handle_error("getcontext"); }
	newtcb->context->uc_stack.ss_sp = malloc(32000);
	newtcb->context->uc_stack.ss_size = 32000;
	newtcb->context->uc_link = &schedule_context;
	makecontext(newtcb->context, function , 0);

	if(schedule_created){
		schedule_created++;
		schedule_context.uc_stack.ss_sp = &schedule_stack;
		schedule_context.uc_stack.ss_size = 32000;
		schedule_context.uc_link = &schedule_context;
		makecontext(&schedule_context, schedule ,0 );
		
		initRunqueue(newtcb->thread_id, newtcb);
		push(&schedule_context, 0);
		push(&uctx_main, 1);

		
	}

    return newtcb->thread_id;
};

/* give CPU possession to other user-level threads voluntarily */
int mypthread_yield() {

	// change thread state from Running to Ready
	// save context of this thread to its thread control block
	// wwitch from thread context to scheduler context

	// YOUR CODE HERE
	return 0;
};

/* terminate a thread */
void mypthread_exit(void *value_ptr) {
	// Deallocated any dynamic memory created when starting this thread

	// YOUR CODE HERE
};


/* Wait for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr) {

	// wait for a specific thread to terminate
	// de-allocate any dynamic memory created by the joining thread

	// YOUR CODE HERE
	return 0;
};

/* initialize the mutex lock */
int mypthread_mutex_init(mypthread_mutex_t *mutex,
                          const pthread_mutexattr_t *mutexattr) {
	//initialize data structures for this mutex


	// YOUR CODE HERE
	return 0;
};

/* aquire the mutex lock */
int mypthread_mutex_lock(mypthread_mutex_t *mutex) {
        // use the built-in test-and-set atomic function to test the mutex
        // if the mutex is acquired successfully, enter the critical section
        // if acquiring mutex fails, push current thread into block list and //
        // context switch to the scheduler thread

        // YOUR CODE HERE
        return 0;
};

/* release the mutex lock */
int mypthread_mutex_unlock(mypthread_mutex_t *mutex) {
	// Release mutex and make it available again.
	// Put threads in block list to run queue
	// so that they could compete for mutex later.

	// YOUR CODE HERE
	return 0;
};


/* destroy the mutex */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex) {
	// Deallocate dynamic memory created in mypthread_mutex_init

	return 0;
};

/* scheduler */
static void schedule() {
	// Every time when timer interrup happens, your thread library
	// should be contexted switched from thread context to this
	// schedule function

	// Invoke different actual scheduling algorithms
	// according to policy (STCF or MLFQ)

	// if (sched == STCF)
	//		sched_stcf();
	// else if (sched == MLFQ)
	// 		sched_mlfq();

	// YOUR CODE HERE

// schedule policy
#ifndef MLFQ
	// Choose STCF
#else
	// Choose MLFQ
#endif

}

/* Preemptive SJF (STCF) scheduling algorithm */
static void sched_stcf() {
	// Your own implementation of STCF
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE
}

/* Preemptive MLFQ scheduling algorithm */
static void sched_mlfq() {
	// Your own implementation of MLFQ
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE
}

// Feel free to add any other functions you need

// YOUR CODE HERE
