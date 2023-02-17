CC=gcc
CFLAGS=-I.

all: server client

server: server.c
	$(CC) -o server tcp_server.c -I.

client: client.c
	$(CC) -o client tcp_client.c -I.
