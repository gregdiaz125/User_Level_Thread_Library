#ifndef MYERRORNUM_H_
#define MYERRORNUM_H_
typedef enum _myerronum{InitConext} myErrorNum;
#include <stdio.h>
    void myerro(int num){
        printf("\n");
        switch (num)
        {
        case InitConext:
            printf("Context was not properly initalized");
            break;
        
        
        }
        printf("\n");
        exit(0);
    }

#endif
