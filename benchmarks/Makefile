CC = gcc
CFLAGS = -g -w -Wall -Wextra -ggdb -O0

all:: parallel_cal vector_multiply external_cal test

parallel_cal:
	$(CC) $(CFLAGS) -pthread -o parallel_cal parallel_cal.c -L../ -lmypthread

vector_multiply:
	$(CC) $(CFLAGS) -pthread -o vector_multiply vector_multiply.c -L../ -lmypthread

external_cal:
	$(CC) $(CFLAGS) -pthread -o external_cal external_cal.c -L../ -lmypthread

test:
	$(CC) $(CFLAGS) -pthread -o test test.c -L../ -lmypthread
test2:
	$(CC) $(CFLAGS) ../mypthread.c ../mypthread.h -o test test.c
testall:
	./parallel_cal 4
	./parallel_cal 8
	./parallel_cal 12
	./parallel_cal 24
	./parallel_cal 50
	./parallel_cal 75
	./parallel_cal 100
	./vector_multiply 4
	./vector_multiply 8
	./vector_multiply 12
	./vector_multiply 24
	./vector_multiply 50
	./vector_multiply 75
	./vector_multiply 100
	./external_cal 4
	./external_cal 8
	./external_cal 12
	./external_cal 24
	./external_cal 50
	./external_cal 75
	./external_cal 100	
clean:
	rm -rf testcase test parallel_cal vector_multiply external_cal *.o ./record/
