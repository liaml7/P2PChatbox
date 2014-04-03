#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "error.h"
#include "crypt.h"
#include "file.h"
#include "echo.h"

void *client(void* v){
	int sockfd, portno, n, x;
	char message[256], *host, ch;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	printf("Enter a message: ");
	scanf("%s", message);
	portno = 111;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	checkError(sockfd, "ERROR opening socket");
	host = fileRead("tracker.txt", host);
	echo(host);
	host = "localhost";
	echo(host);
	server = gethostbyname(host);
	checkNNull(server, "ERROR, no such host");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	checkError(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)), "ERROR connecting");
	n = write(sockfd,message,strlen(message));
	checkError(n, "ERROR writing to socket"); 
	bzero(message,256);
	n = read(sockfd,message,255);
	checkError(n, "ERROR reading from socket");
	echo(message);
	close(sockfd);
	for(x = 0; x < 1; x++){
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		checkError(sockfd, "ERROR opening socket");
		strcpy(message, "test");
		server = gethostbyname(host);
		checkNNull(server, "ERROR, no such host");
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(portno);
		checkError(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)), "ERROR connecting");
		n = write(sockfd,message,strlen(message));
		checkError(n, "ERROR writing to socket");
		bzero(message,256);
		n = read(sockfd,message,255);
		checkError(n, "ERROR reading from socket");
		echo(message);
		close(sockfd);
	}
}

void *server(void* v){
	int sockfd, newsockfd, portno, n, x;
	char buffer[256];
	portno = 111;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	checkError(sockfd, "ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	checkError(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)), "ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	for(x = 0; x < 2; x++){
		//wait for socket reply
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		checkError(newsockfd, "ERROR on accept");
		bzero(buffer,256);
		//read data from socket and save to buffer variable
		n = read(newsockfd,buffer,255);
		checkError(n, "ERROR reading from socket");
		if(strncmp(buffer,"fetch",5) == 0){
			printf("Fetching \n");
		}
		n = write(newsockfd,"I got your message",18);
		checkError(sockfd, "ERROR writing to socket");
	}
	close(newsockfd);
	close(sockfd);
	exit(0);
}

void main(){
	//create two threads for client and server, join causes a wait until both threads end
	pthread_t clientthread, serverthread;
	pthread_create(&clientthread, NULL, client, NULL);
	pthread_create(&serverthread, NULL, server, NULL);
	pthread_join(clientthread, NULL);
	pthread_join(serverthread, NULL);
}
