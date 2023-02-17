CC=gcc
CFLAGS=-I.
all: server client

server: tcp_server.c
	$(CC) -o server tcp_server.c -I.

client: tcp_client.c
	$(CC) -o client tcp_client.c -I.
