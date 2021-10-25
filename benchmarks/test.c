#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "../mypthread.h"

/* A scratch program template on which to call and
 * test mypthread library functions as you implement
 * them.
 *
 * You can modify and use this program as much as possible.
 * This will not be graded.
 */	
void test1(){
	mypthread_yield();
	int i,a = 0 ;
	for( i = 0;i != 2147483000; i++){
		a= a+1;
	}
	printf("\nthis is thread 2\n");
	mypthread_exit(NULL);
}

void test2(){
	int i,a = 0 ;
	for( i = 0;i != 2147483000; i++){
		a= a+1;
	}
	printf("\nthis is thread 2\n");
}

void test3(){
	int i,a = 0 ;
	for( i = 0;i != 2147483000; i++){
		a= a+1;
	}
	mypthread_exit(NULL);
	printf("\nthis is thread 3\n");
}


int  test4(int * b){
	mypthread_yield();
	mypthread_yield();
	int * a = malloc(sizeof(int));
	printf("\n the integer is %d\n",*b);
	*a = (*b) * 2;
	(*a) = (*a) * 2;
	printf("\n the integer is %d\n",*a);
	mypthread_exit(a);
}

int main(int argc, char **argv) {
	int a = 5;
	int * ret;
	printf("\nmain functionis running\n");
	pthread_t  thread1, thread2, thread3, thread4;
	mypthread_create(&thread4, NULL, test4, &a);
	mypthread_create(&thread2, NULL, test2, NULL);
	mypthread_create(&thread3, NULL,test3, NULL);
	mypthread_join(thread4,&ret);
	
	printf("\nreturn value was %d\n", *ret);
	printf("\nmain is finished\n");
	return 0;
}
