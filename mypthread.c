// File:	mypthread.c

// List all group member's name: Gregory Diaz
// username of iLab: gldd55
// iLab Server:

#include "mypthread.h"
// INITAILIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE


/*
		--------------- Thread Creation ---------------
*/
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg) {
	tcb* new_tcb = createnew_tcb();
	if(getcontext(new_tcb->context) == -1){ handle_error("getcontext");}
	makecontext(new_tcb->context, function , 1, arg);
	*thread = new_tcb->thread_id;
	enQueue(&thread_pool,new_tcb);
	enQueue(&runQueue,new_tcb);
	if(!schedule_created){
		create_schedule_ctx();
		create_exit_ctx();
		init_sigact();
		set_timer();
	}
    return *thread;
}

void create_schedule_ctx(){
	schedule_created++;
	schedule_ctx.uc_stack.ss_sp = &schedule_stack;
	schedule_ctx.uc_stack.ss_size = 32000;
	schedule_ctx.uc_link = NULL;
	getcontext(&schedule_ctx);
	makecontext( &schedule_ctx, schedule, 0 );
}

void create_exit_ctx(){
	exit_ctx.uc_stack.ss_sp = &no_exit_stack;
	exit_ctx.uc_stack.ss_size = 32000;
	exit_ctx.uc_link = NULL;
	getcontext(&exit_ctx);
	makecontext(&exit_ctx, mypthread_exit,1, NULL);
}

tcb* createnew_tcb(){
	tcb* temp = (tcb*) malloc(sizeof(tcb));
	temp->thread_id = __sync_fetch_and_add(&next_thread_id, 1);
	temp->status= READY;
	temp->waiting_thread = NULL;
	temp->join_ptr = NULL;
	temp->return_ptr = NULL;
	temp->time_elapsed = 0;
	temp->context = (ucontext_t*) malloc(sizeof(ucontext_t));
	temp->context->uc_stack.ss_sp = malloc(32000);
	temp->context->uc_stack.ss_size = 32000;
	temp->context->uc_link = &exit_ctx;
	return temp;
}


/*
		--------------- Thread Minipulation ---------------
*/

/* give CPU possession to other user-level threads voluntarily */
int mypthread_yield() {
	swapcontext((running_thread->context),&schedule_ctx);
	return 0;
};
/*default uc_link for new thread that are being created wil handle the Deallotating*/
	
/* terminate a thread */
void mypthread_exit(void *value_ptr) {
	// Deallocated any dynamic memory created when starting this thread
	atomic_flag_test_and_set(&e_lock);
	//check to see is the join thread is done?
	if(running_thread->waiting_thread != NULL){
		tcb* wainting_thread = running_thread->waiting_thread;
		wainting_thread->status = READY;
		enQueue(&runQueue, wainting_thread);
		if(value_ptr != NULL){
			*(wainting_thread->join_ptr) = value_ptr;
		}
		free_thread(running_thread);
		setcontext(&schedule_ctx);
	}
	if(value_ptr == NULL){
		//check the status of the running thread;
		free_thread(running_thread);
		setcontext(&schedule_ctx);
	}
	running_thread ->status = FINISHED;
	running_thread ->return_ptr = value_ptr;
	atomic_flag_clear(&e_lock);
	swapcontext(running_thread->context,&schedule_ctx);
	
}

/* Wait for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr) {
	atomic_flag_test_and_set(&e_lock);
	tcb* temp = searchQueue(&thread_pool, thread);
	if(temp == NULL){
		if(value_ptr !=NULL){
			handle_error("Thread exited but Join is waiting for return");
			}
		return 0;
	}		 
	if(temp->status == FINISHED){
		if(value_ptr != NULL){
			*value_ptr = temp->return_ptr;
			free_thread(temp);
		}
		return 0;
	}
	temp->waiting_thread = running_thread;
	running_thread->join_ptr = value_ptr;
	running_thread->status = WAITING;
	atomic_flag_clear(&e_lock);
	swapcontext(running_thread->context, &schedule_ctx);
	return 0;
};

void free_thread(tcb* exit_thread){
	removefromQueue(&thread_pool, exit_thread);
	atomic_flag_test_and_set(&s_lock);
	if(exit_thread->thread_id = running_thread->thread_id){
		running_thread = NULL;
	}
		free(exit_thread->context->uc_stack.ss_sp);
		free(exit_thread->context);
		free(exit_thread);
		atomic_flag_clear(&s_lock);
		
}

/*	--------------- Mutexes ---------------*/
	
int mypthread_mutex_init(mypthread_mutex_t *mutex,
                          const pthread_mutexattr_t *mutexattr) {
	atomic_flag lock = ATOMIC_FLAG_INIT;
	return 0;
};

int mypthread_mutex_lock(mypthread_mutex_t *mutex) {
	while(atomic_flag_test_and_set(&(mutex->lock))){
		swapcontext(running_thread->context, &schedule_ctx);
	}
        return 0;
};


int mypthread_mutex_unlock(mypthread_mutex_t *mutex) {
	atomic_flag_clear( &(mutex->lock) );
	return 0;
};


int mypthread_mutex_destroy(mypthread_mutex_t *mutex) {
	return 0;
};

/*--------------- Scheduler ---------------*/
		
static void schedule() {
	stop_timer();
	if(running_thread != NULL){
		if(running_thread->status == RUNNING){
			running_thread->status = READY;
			enQueue(&runQueue, running_thread);
		}
	}
	running_thread = sched_stcf();
	running_thread->status = RUNNING;
	set_timer();
	setcontext(running_thread->context);
	printf("the scheduler context finished\n");	


}

/* Preemptive SJF (STCF) scheduling algorithm */
tcb * sched_stcf() {
	Queue * q = &runQueue;
	QNode * temp = q->front;
	tcb* target = temp->tcb_ptr;
	while(temp->next != NULL){
		if(target->time_elapsed > temp->tcb_ptr->time_elapsed){
			target = temp->tcb_ptr;
		}
		temp = temp->next;
	}
	removefromQueue(&runQueue, target);
	return(target);
}

/* Preemptive MLFQ scheduling algorithm */
tcb * sched_fcfs() {
	return(deQueue(&runQueue));
}


/*		--------------- Queue ---------------*/

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
	atomic_flag_test_and_set(&s_lock);
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
	atomic_flag_clear(&s_lock);
}

tcb* deQueue(Queue * q){
	atomic_flag_test_and_set(&s_lock);
	if (q->front == NULL){
		return NULL;
	}
	tcb* tcb_temp = q->front->tcb_ptr;
	QNode* temp = q->front;
	q->front = q->front->next;
	if (q->front == NULL){
        q->rear = NULL;
	}
	free(temp);
	return(tcb_temp);
	atomic_flag_clear(&s_lock);
}

tcb* searchQueue(Queue* q, int id){
	QNode* temp;
	atomic_flag_test_and_set(&s_lock);
	for(temp= q->front; temp!= NULL; temp=temp->next){
		if( id == temp->tcb_ptr->thread_id){
			break;
		}
	}
	atomic_flag_clear(&s_lock);
	if(temp == NULL){
		return(NULL);
	}
	tcb* target = temp->tcb_ptr;
	return(target);
}

void printQueue(Queue * q){
	QNode* temp;
	int i = 0;
	for(temp= q->front; temp!= NULL; temp=temp->next){
		printf("%d, ",temp->tcb_ptr->thread_id);
	}
	printf("\n");
}

void removefromQueue(Queue *q, tcb* target){
	atomic_flag_test_and_set(&s_lock);
	QNode* prev,* temp = q->front;
	if(q->front->tcb_ptr == target){
		q->front = q->front->next;
		free(temp);
		return;
	}
	while(temp != NULL){
		prev = temp;
		temp = prev->next;
		if(temp->tcb_ptr == target ){break;}
	}
	if(temp == NULL){handle_error("thread was not in queue for removal");}
	prev->next= temp->next;
	free(temp);
	atomic_flag_clear(&s_lock);
}

void init_sigact(){
	struct sigaction act = {.sa_flags = 0, 
							.sa_handler = timer_interrupt};
	sigemptyset(&act.sa_mask);
	sigaction(SIGALRM, &act, NULL);
};

void stop_timer() {
    struct itimerval value;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = 0;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &value, NULL) == -1) {
        handle_error("stop_timer");
    }
};

void set_timer() {
    struct itimerval value;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = 0;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = THREAD_QUANTUM;

    if (setitimer(ITIMER_REAL, &value, NULL) == -1) {
        handle_error("set_timer");
    }
};

void timer_interrupt(int sig){
	if(!(atomic_flag_test_and_set( &e_lock )) && running_thread != NULL&& !(atomic_flag_test_and_set( &s_lock ))){
		running_thread->time_elapsed++;
		atomic_flag_clear( &s_lock);
		atomic_flag_clear( &e_lock );
		swapcontext(running_thread->context,&schedule_ctx);
		return;
	}
		set_timer();
		return;	
	
}