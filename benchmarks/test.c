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
	printf("\n hello world \n");
}

void test2(int * a){
	printf("\n the integer is %d\n",a);
}

int main(int argc, char **argv) {
	int a = 5;
	pthread_t *thread = (pthread_t*) malloc(sizeof(mypthread_t));
	pthread_create(&thread, NULL, (*test1), NULL);
	printf("thread created\n");

	return 0;
}
