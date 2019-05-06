/**
 * @author Andrew Delgado and Christian Baiza
 * @date 5-5-2019
 * @version 1.0
 *
 * This is a client program that executes a TCP and UDP connection. The TCP connection is for sending and recieving files from the server in a p2p network. The UDP connection acts as a registration protocol for the server.
 */
// UDP client program 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <pthread.h> 
#define PORT 8080 
#define MAXLINE 1024 

void udpCall();
void tcpCall();

char *GUID = "";

int main() 
{ 
    pthread_t thread_id[2];

    pthread_create(&thread_id[0], NULL,tcpCall,NULL);
    sleep(1);
    pthread_create(&thread_id[1], NULL,udpCall,NULL);

    printf("Printing this from the main: GUID: %s", GUID);

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    
    return 0; 
} 


void tcpCall()
{
int sockfd; 
    char buffer[MAXLINE]; 
    char* message = "test.txt"; 
    struct sockaddr_in servaddr; 

    int n, len; 
    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("socket creation failed"); 
        exit(0); 
    } 

    memset(&servaddr, 0, sizeof(servaddr)); 

    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    if (connect(sockfd, (struct sockaddr*)&servaddr, 
                            sizeof(servaddr)) < 0) { 
        printf("\n Error : Connect Failed \n"); 
    } 
    memset(buffer, 0, sizeof(buffer));
    while((n = read(sockfd, buffer, sizeof(buffer)-1)) > 0)
    {
        //memset(message, 0, sizeof(message));
        //memset(buffer, 0, sizeof(buffer));
        GUID = buffer;
        // printf("Message from server: "); 
        //printf(""); 
        if(fputs(buffer, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
        printf("\n");
        send(sockfd, message, sizeof(message),0);
    }
    
    if(n < 0)
        {
            printf("\n Read error \n");
        }
    return 0;
}

void udpCall()
{
    int sockfd; 
    char buffer[MAXLINE]; 
    char* message = GUID; 
    //char message[100];
    struct sockaddr_in servaddr; 

    int n, len; 
    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
        printf("socket creation failed"); 
        exit(0); 
    } 

    memset(&servaddr, 0, sizeof(servaddr)); 

    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        //printf("Test\n"); 
    //printf("\nEnter in a message: ");
    //fgets(message, 100, stdin);
    // send hello message to server 
    while(1){

    
    sendto(sockfd, (const char*)message, strlen(message), 
        0, (const struct sockaddr*)&servaddr, 
        sizeof(servaddr)); 
        sleep(2);
    }
    //receive server's response 
    // printf("Message from server: "); 
    // n = recvfrom(sockfd, (char*)buffer, MAXLINE, 
    //          0, (struct sockaddr*)&servaddr, 
    //          &len); 
    // puts(buffer); 
    close(sockfd); 
}
