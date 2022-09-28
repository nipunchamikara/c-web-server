CC = gcc
CFLAGS = -g -Wextra -Wall 

server: server.c
	$(CC) $(CFLAGS) -o server server.c

build: server

run: server
	./server
