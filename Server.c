/**
 * @author Andrew Delgado and Christian Baiza
 * @date 5-5-2019
 * @version 1.0
 *
 * This program acts as a server to facilitate and register clients for a p2p network
 * This is achieved by setting up a socket with the port 8080,
 * 	the program then waits and listens for a client trying to connect.
 * Once the client connects to the server it registers the client with a GUID and accepts the file passed to it.
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
#define PORT 8080
#define MAXLINE 1024


struct users{
	int GUID;
	char fileName[1024];
	int timeStamp;

};

void udpCall();
void tcpCall(struct users list[], size_t len);
long random();
int check(int clientTime);

int main(int argc, char const *argv[])
{
	struct users list[10];

	pthread_t thread_id[2];
	pthread_create(&thread_id[0], NULL,udpCall,list);
	pthread_create(&thread_id[1], NULL,tcpCall,list);

	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	return 0;
}

void udpCall(struct users list[], size_t len){
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

		int num = atoi(buffer);
		
		for(int i = 0; i < sizeof(list)/sizeof(int); i++){
			if(list[i].GUID == num){
				int answer = check(list[i].GUID);
				printf("Client GUID: %s\n", buffer); 
				sendto(sockfd, (const char *)hello, strlen(hello),  
				MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
		printf("-----------------UDP Connection Check....time: %d\n\n", time(0));  
			}
		}

	 }
}

void tcpCall(struct users list[], size_t len) {

	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char serverMessage[200] = "";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

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
	int i = 0;
	while(i < 5)//This is an infinite loop that keeps the chat indefinitely open
	{
		new_socket = accept(server_fd, (struct sockaddr*)NULL, NULL);
		//clears the buffer
		if(strlen(serverMessage)> 0)
		{
			memset(serverMessage, 0, sizeof(serverMessage));
			memset(buffer, 0, sizeof(buffer));
		}

		list[i].GUID = random();
		list[i].timeStamp = time(0);

		snprintf(serverMessage, sizeof(serverMessage), "%d", list[i].GUID);
		write(new_socket, serverMessage, strlen(serverMessage));
		valread = read( new_socket , buffer, 1024);
		strcpy(list[i].fileName, buffer);

                printf("TCP Connection Initialized Client: %d\n", list[i].GUID);

		printf("GUID: %d FileName: %s timeStamp: %d\n\n", list[i].GUID, list[i].fileName,list[i].timeStamp );
	i++;
	}
	close(new_socket);
}

int check(int clientTime){

long currentTime = time(0);

if((currentTime - clientTime) > 200){
	return 1;	
}
return 0;

}

long random(){
	srand(time(0));
	return rand()%1001;
}

