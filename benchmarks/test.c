#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "../mypthread.h"

/* A scratch program template on which to call and
 * test mypthread library functions as you implement
 * them.
 *
 * You can modify and use this program as much as possible.
 * This will not be graded.
 */	
void test1(){
	printf("\n this is thread 1 \n");
	return 0;
}

void test2(int * a){
	printf("\n the integer is %d\n",a);
}
void test3(){
	printf("\nthis is thread 2\n");
}

void test4(){
	printf("\nthis is thread 3\n");
}

int main(int argc, char **argv) {
	int a = 5;
	pthread_t *thread = (pthread_t*) malloc(sizeof(mypthread_t));
	pthread_create(&thread, NULL, (*test1), NULL);
	pthread_create(&thread, NULL, (*test2), NULL);
	pthread_create(&thread, NULL, (*test3), NULL);
	pthread_create(&thread, NULL, (*test4), NULL);
	printf("main is finished\n");
	return 0;
}
