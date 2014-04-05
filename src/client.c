#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


void error(const char *msg);

void parse(char host[BUFSIZE], int *portno);


int main(int argc, char *argv[]) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char host[BUFSIZ];

  parse(host, &portno);

	char message[256];
	printf("Enter a message: ");
	scanf("%s", message);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");

	server = gethostbyname(host);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
		error("ERROR connecting");
	}
	n = write(sockfd,message,strlen(message));
	if (n < 0){ 
		error("ERROR writing to socket");
	}
	memset(message, 0, 256);
	n = read(sockfd,message,255);
	if (n < 0){
		error("ERROR reading from socket");
	}
	printf("%s\n",message);
	close(sockfd);
	int x;
	for(x = 0; x < 1; x++){
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
			error("ERROR opening socket");
		strcpy(message, "test");
		server = gethostbyname(host);
		if (server == NULL) {
			fprintf(stderr,"ERROR, no such host\n");
			exit(0);
		}
		memset((char *) &serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(portno);
		if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
			error("ERROR connecting");
		}
		n = write(sockfd,message,strlen(message));
		if (n < 0){ 
			error("ERROR writing to socket");
		}
		memset(message, 0, 256);
		n = read(sockfd,message,255);
		if (n < 0){
			error("ERROR reading from socket");
		}
		printf("%s\n",message);
		close(sockfd);
	}
	return 0;
}


void parse(char host[BUFSIZE], int *portno) {
  FILE *parsefile;

    parsefile = fopen("tracker.dat", "r");

  fscanf(parsefile, " host: %s", host);
  fscanf(parsefile, " port: %d", portno);
  fclose(parsefile);
}

void error(const char *msg) {
    perror(msg);
    exit(0);
}