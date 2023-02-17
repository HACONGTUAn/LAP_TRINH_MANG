CC=gcc
CFLAGS=-I.

all: server client

server: server-socket.c
	$(CC) -o server server-socket.c -pthread.

client: client-socket.c
	$(CC) -o client client-socket.c -pthread.
