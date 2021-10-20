// File:	mypthread.c

// List all group member's name: Gregory Diaz
// username of iLab: gldd55
// iLab Server:

#include "mypthread.h"
#include <ucontext.h>
// INITAILIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE

static ucontext_t schedule_ctx, main_ctx, no_exit_ctx;
static tcb main_tcb; 
static char schedule_stack[32000], no_exit_stack[32000];
static bool schedule_created = FALSE;
static bool schedule_space_lock = FALSE;


/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr,
                      void *(*function)(void*), void * arg) {
	//schedule_space_lock = TRUE;
	tcb* newtcb = createNewTCB();
	if(getcontext(newtcb->context) == -1){ handle_error("getcontext"); }
	newtcb->context->uc_stack.ss_sp = malloc(32000);
	newtcb->context->uc_stack.ss_size = 32000;
	newtcb->context->uc_link = &no_exit_ctx;
	makecontext(newtcb->context, function , 0);

	if(!schedule_created){
		create_schedule_ctx();
		create_no_exit_ctx();
		main_tcb.thread_id = 1;
		main_tcb.status = READY;
		main_tcb.context = &main_ctx;
		//runQueue = createQueue;
		enQueue(&runQueue,newtcb);
		enQueue(&runQueue,&main_tcb);
		swapcontext(&main_ctx, &schedule_ctx);
	}else{
		enQueue(&runQueue,newtcb);
		swapcontext(((&runQueue)->front->tcb_ptr),&schedule_ctx);
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
	if(value_ptr == NULL){
		printf("this thread didnt use exit\n");
	}
	tcb* exit_thread = runningThread;
	runningThread = NULL;
	setcontext(&schedule_ctx);
}
	// YOUR CODE HERE

/*	default uc_link for new thread that are being created
	wil handle the Deallotating
 */
void mypthread_no_exit(){
	/* do not de allocate anything yet just switch to scheduler*/
	tcb* exit_thread = runningThread;
	free(exit_thread->context->uc_stack.ss_sp);
	free(exit_thread->context);
	free(exit_thread);
	runningThread = NULL;
	setcontext(&schedule_ctx);
	printf("thread exit has finished");
}


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
	if(runningThread != NULL){
		enQueue(&runQueue, runningThread);
	}
	runningThread = deQueue(&runQueue);
	setcontext(runningThread->context);

	printf("the scheduler context finished\n");	

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
void threadpool(int id, tcb* tb){
	front = (poolnode*) malloc(sizeof(poolnode));
	front->previous = front;
	front->next = front;
	front->threadid = id;
}

void push(int id,tcb* tb ){
	if(front == NULL){ handle_error("queueNULL");}
	poolnode *newNode = (poolnode*) malloc(sizeof(poolnode));
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
	poolnode *temp = front;
	free(front);
	front = temp;
}

void create_schedule_ctx(){
	schedule_created++;
	schedule_ctx.uc_stack.ss_sp = &schedule_stack;
	schedule_ctx.uc_stack.ss_size = 32000;
	schedule_ctx.uc_link = &schedule_ctx;
	getcontext(&schedule_ctx);
	makecontext(&schedule_ctx, schedule ,0);
}

void create_no_exit_ctx(){
	no_exit_ctx.uc_stack.ss_sp = &no_exit_stack;
	no_exit_ctx.uc_stack.ss_size = 32000;
	no_exit_ctx.uc_link = &schedule_ctx;
	getcontext(&no_exit_ctx);
	makecontext(&no_exit_ctx, mypthread_exit,1, NULL);
}

tcb* createNewTCB(){
	tcb* temp = (tcb*) malloc(sizeof(tcb));
	temp->thread_id = next_thread_id;
	next_thread_id++;
	temp->status= READY;
	temp->context = (ucontext_t*) malloc(sizeof(ucontext_t));
	return temp;
}


QNode* newNode(tcb* new_ptr){
	QNode* temp =(QNode*)malloc(sizeof(QNode));
	temp->tcb_ptr = new_ptr;
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

tcb* deQueue(Queue * q){
	if (q->front == NULL){
		return;
	}
	tcb* tcb_temp = q->front->tcb_ptr;
	QNode* temp = q->front;
	q->front = q->front->next;
	if (q->front == NULL){
        q->rear = NULL;
	}
	free(temp);
	return(tcb_temp);
}
