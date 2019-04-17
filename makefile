CC1=gcc -O3 -Wall -pthread

CFLAGS = -g -c -Wall
#CFLAGS = -ansi -c -Wall -pedantic
all: TCP_Client Server UDP_Client

A = TCP_Client.o
B = Server.o
C = UDP_Client.o

TCP_Client: $A
	$(CC1) -o TCP_Client $A

TCP_Client.o: TCP_Client.c
	$(CC1) -c TCP_Client.c

Server: $B
	$(CC1) -o Server $B

Server.o: Server.c
	$(CC1) -c Server.c

UDP_Client: $C
	$(CC1) -o UDP_Client $C

UDP_Client.o: UDP_Client.c
	$(CC1) -c UDP_Client.c

clean:
	rm *.o
	rm Server
	rm TCP_Client
	rm UDP_Client        

