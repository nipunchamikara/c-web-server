CC = gcc
CFLAGS = -Wall -g

server: 
	$(CC) $(CFLAGS) -o server server.c

build: server

run: server
	./server
