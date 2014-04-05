#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif
#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int socketCreate(){
	return socket(AF_INET, SOCK_STREAM, 0);
}
int socketWrite(int sockfd, char *message){
	return write(sockfd,message,strlen(message));
}
int socketRead(int sockfd, char *message){
	return read(sockfd,message,255);
}
void socketClose(int sockfd){
	close(sockfd);
}
int socketGetPort(){
	return 112;
}
