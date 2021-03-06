#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif
#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

void error(const char *msg){
    perror(msg);
    exit(0);
}
void checkError(int val, char *msg){
	if(val < 0){
		error(msg);
	}
}
void checkNNull(struct hostent *val, char *msg){
	if(val == NULL){
		error(msg);
	}
}
