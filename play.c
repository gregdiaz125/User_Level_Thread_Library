#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
int func1(){


    return 6;
}
int  test4(int * b){
	int * a = malloc(sizeof(int));
	printf("\n the integer is %d\n",*b);
	*a = (*b) * 2;
	(*a) = (*a) * 2;
    return(*a);
}

void func2(int * a){
    free(a);
    a = NULL;
}
int main(int argc,char ** argv){
    int *b = malloc(sizeof(int));
    *b = 5;
    func2(b);
    if( b == NULL){
        printf("\nb is null\n");
    }
    free(b);
    printf("\nthis program ran\n");
    return 0;
}