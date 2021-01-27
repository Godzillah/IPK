# DESCRIPTION :	makefile for client/server application protocol 
# AUTHOR 	  : MAROS ORSAK
# DATE   	  : 18.2.2018


libs 			= client_server.h
cflags 			= -w -Wextra -Wall -std=c11 

all: server client

rebuild: clean server client

client: 
	gcc $(libs) $(cflags) ipk-client.c -o ipk-client 
server:
	gcc $(libs) $(cflags) ipk-server.c -o ipk-server

clean:
	rm ipk-client
	rm ipk-server
zip:
	rm ipk-client
	rm ipk-server
	zip protocol.zip ipk-client.c ipk-server.c client_server.h Copyright Milniky readme

unzip:
	unzip protocol.zip ipk-client.c ipk-server.c client_server.h Copyright Milniky readme
