CC	= g++

all : server client

server : main.c server.c server.h server.o
	 cc -o server -pthread main.c server.o -pthread

client : client.c client.o
	 cc -o client client.o

server.o : server.c server.h
	   cc -c server.c

client.o : client.c
	   cc -c client.c

clean:
	   rm -rf *o server client *~
