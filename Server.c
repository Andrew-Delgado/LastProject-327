/**
 * @author Andrew Delgado and Christian Baiza
 * @date 2-24-2019
 * @version 1.0
 *
 * This program acts as a server to receive and send messages to a client(Client.c).
 * This is achieved by setting up a socket with the port 8080,
 * 	the program then waits and listens for a client trying to connect.
 * Once the client connects to the server it waits for the client to send the first message,
 * 	the message is then displayed in the terminal and then each program must take turns
 * 	sending messages.
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <pthread.h> 
#define PORT 5051
#define MAXLINE 1024


struct users{
	int GUID;
	char fileName[1024];
	int timeStamp;

};

void udpCall();
void tcpCall(struct users list[], size_t len);
long random();

int main(int argc, char const *argv[])
{
	struct users list[10];

	pthread_t thread_id[2];
	pthread_create(&thread_id[0], NULL,udpCall,NULL);
	pthread_create(&thread_id[1], NULL,tcpCall,list);

	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	//udpCall();

	//tcpCall(list, sizeof(list) / sizeof(*list));
	

	return 0;
}

void udpCall(){
	int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
	char exitWord[1024] = "goodbye";
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

	//added this to keep the UDP_Checking.
	 while(1)
	 {
		int len, n; 
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
					MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
					&len); 
		buffer[n] = '\0'; 

		// if(strcmp(exitWord, buffer) == 0){
		// 	break;
		// }

		printf("Client : %s\n", buffer); 
		sendto(sockfd, (const char *)hello, strlen(hello),  
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
				len); 
		printf("-----------------message sent from server....time: %d\n", time(0));  
	 }
}

// void udpCall(){

// 	int listenfd, connfd, udpfd, nready, maxfdp1; 
// 	char buffer[MAXLINE]; 
// 	pid_t childpid; 
// 	fd_set rset; 
// 	ssize_t n; 
// 	socklen_t len; 
// 	const int on = 1; 
// 	struct sockaddr_in cliaddr, servaddr; 
// 	char* message = "Hello Client"; 
// 	void sig_chld(int); 

// 	printf("before the infinite loop");

// 	udpfd = socket(AF_INET, SOCK_DGRAM, 0); 
// 	// binding server addr structure to udp sockfd 
// 	bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

// 	// clear the descriptor set 
// 	FD_ZERO(&rset); 

// 	//get maxfd; 
// 	maxfdp1 = udpfd; 

// 	// 	// set listenfd and udpfd in readset 
// 		FD_SET(listenfd, &rset); 
// 		FD_SET(udpfd, &rset); 

// 		// select the ready descriptor 
// 		nready = select(maxfdp1, &rset, NULL, NULL, NULL); 

// 	printf("before the infinite loop");
// 	// if udp socket is readable receive the message. 
// 	while(1)
// 	{
// 		if (FD_ISSET(udpfd, &rset)) { 
// 			len = sizeof(cliaddr); 
// 			bzero(buffer, sizeof(buffer)); 
// 			printf("\nMessage from UDP client: "); 
// 			n = recvfrom(udpfd, buffer, sizeof(buffer), 0, 
// 						(struct sockaddr*)&cliaddr, &len); 
// 			puts(buffer); 
// 			//sendto(udpfd, (const char*)message, sizeof(buffer), 0, 
// 			//	(struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
// 		}
// 	}
// }


void tcpCall(struct users list[], size_t len) {

	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char serverMessage[200] = "This is from the server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// // Forcefully attaching socket to the port 8080
	// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	// 											&opt, sizeof(opt)))
	// {
	// 	perror("setsockopt");
	// 	exit(EXIT_FAILURE);
	// }
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	while(1)//This is an infinite loop that keeps the chat indefinitely open
	{

		//clears the buffer
		if(strlen(serverMessage)> 0)
		{
			memset(serverMessage, 0, sizeof(serverMessage));
			memset(buffer, 0, sizeof(buffer));
		}

		valread = read( new_socket , buffer, 1024);
		printf("client: %s\n",buffer );


		list[0].GUID = random();
		strcpy(list[0].fileName, buffer);
		list[0].timeStamp = time(0);

		printf("Enter Message: ");
		fgets(serverMessage, 200,stdin);
		send(new_socket , serverMessage , strlen(serverMessage) , 0 );

		//break;
	}

	printf("GUID: %d FileName: %s timeStamp: %d\n", list[0].GUID, list[0].fileName,list[0].timeStamp );
}

long random(){

	srand(time(0));



	return rand()%1001;
}