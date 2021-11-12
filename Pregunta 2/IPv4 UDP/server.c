#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // read, write, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM,
                       // INADDR_ANY, socket etc...
#include <string.h> // memset

#define MAXLINE 1024

int main(int argc, char const *argv[]) {

	int serverFd;
	struct sockaddr_in server, client;
	socklen_t len;
	int port = 1234;
	char buffer[1024];
	
	if (argc == 2) {
		port = atoi(argv[1]);
	}
	
	serverFd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (serverFd < 0) {
		perror("Cannot create socket");
		exit(1);
	}
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
 
	len = sizeof(server);
	if (bind(serverFd, (struct sockaddr *)&server, len) < 0) {
	perror("Cannot bind sokcet");
		exit(2);
	}
  
	int n;
	
	while (1) {
		
		len = sizeof(client);
		printf("waiting for clients\n");
		 
		memset(buffer, 0, sizeof(buffer));len = sizeof(client);
		  
		n = recvfrom(serverFd,(char *)buffer,MAXLINE,0, (struct sockaddr *) &client,&len);
		  
		buffer[n] = '\0';
		 
		char *client_ip = inet_ntoa(client.sin_addr);
		printf("Accepted new connection from a client %s:%d\n", client_ip, ntohs(client.sin_port));
		   
		printf("Message sent to client.\n");
		sendto(serverFd,(const char *)"Hello client from server !!",strlen("Hello client from server !!"),MSG_CONFIRM,(const struct sockaddr *)&client,len);
		    
		printf("received %s from client\n", buffer);
		
	}
	
	close(serverFd);
	 
	return 0;
}
