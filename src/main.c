#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "socket.h"
#include "echo.h"
#include "error.h"
#include "crypt.h"
#include "file.h"

void *client(void* v);
void *server(void* v);

void main(){
	//create two threads for client and server, join causes a wait until both threads end
	pthread_t clientthread, serverthread;
	pthread_create(&clientthread, NULL, client, NULL);
	pthread_create(&serverthread, NULL, server, NULL);
	pthread_join(clientthread, NULL);
	pthread_join(serverthread, NULL);
}


void *client(void* v){
	//ENCRYPTION VARS
	char * encrypt_handle = NULL;
	char * decrypt_handle = NULL;
	result encrypt_result;
	result decrypt_result;
	const char * data_to_encrypt = "This is an encryption test";
	size_t data_to_encrypt_length = strlen(data_to_encrypt);
	char password[256] = "testtesttest";
	size_t password_length = strlen(password);
	const char * decrypted_data;
	size_t decrypted_data_size;
	const char * encrypted_data;
	size_t encryped_data_size;
	
	int sockfd, portno, n, x;
	char host[BUFSIZ];
	char message[256], ch;
	getChatPassword(password);
	password_length = strlen(password);
	struct sockaddr_in serv_addr;
	struct hostent *server;
	getHost(host);
	server = gethostbyname(host);
	printf("Enter a message: ");
	scanf("%s", message);
	portno = socketGetPort();
	sockfd = socketCreate();
	checkError(sockfd, "ERROR opening socket");
	checkNNull(server, "ERROR, no such host");
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	checkError(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)), "ERROR connecting");
	n = socketWrite(sockfd,message);
	checkError(n, "ERROR writing to socket"); 
	memset(message, 0, 256);
	n = socketRead(sockfd,message);
	checkError(n, "ERROR reading from socket");
	echo(message);
	socketClose(sockfd);
	for(x = 0; x < 1; x++){
		sockfd = socketCreate();
		checkError(sockfd, "ERROR opening socket");
		strcpy(message, "test");
		server = gethostbyname(host);
		checkNNull(server, "ERROR, no such host");
		memset((char *) &serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(portno);
		checkError(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)), "ERROR connecting");
		//ENCRYPT MESSAGE
		if(encrypt(&encrypt_handle,data_to_encrypt,data_to_encrypt_length,password,password_length,&encrypt_result)){
			encrypted_data = encrypt_result.buffer;
			encryped_data_size = encrypt_result.length;
		}else{echo("data encryption failed");}
		////////////////////////////////////////////////////////////////////////////
		//NEED TO REPLACE THE message VAR WITH THE encrypted_data FROM ABOVE
		n = socketWrite(sockfd,message);
		checkError(n, "ERROR writing to socket");
		memset(message, 0, 256);
		n = socketRead(sockfd,message);
		checkError(n, "ERROR reading from socket");
		echo(message);
		socketClose(sockfd);
	}
}


void *server(void* v){
	int sockfd, newsockfd, portno, n, x;
	char buffer[256];
	portno = socketGetPort();
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	checkError(sockfd, "ERROR opening socket");
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
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
		memset(buffer, 0, 256);
		//read data from socket and save to buffer variable
		n = read(newsockfd,buffer,255);
		checkError(n, "ERROR reading from socket");
		if(strncmp(buffer,"fetch",5) == 0){
			printf("Fetching \n");
		}
		strcat(buffer, " is your message");
		n = write(newsockfd,buffer,255);
		checkError(sockfd, "ERROR writing to socket");
	}
	close(newsockfd);
	close(sockfd);
	exit(0);
}
