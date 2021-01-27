#AUTHOR		: MAROS ORSAK
#BRIEF		: makefile...(subor pomocou ktoreho budeme moct prelozit , spustit program)

NAME		= ipk-lookup
CC 			= g++
CFLAGS 		= -Wextra -std=c++11 -o 
OBJECT		= ipk-lookup
CPP			= main.cpp parseArgs.cpp err.cpp dnsHeaderClient.cpp dnsHeaderHeader.cpp querry.cpp rData.cpp responseRecord.cpp
HEADERS 	= err.h parseArgs.h dnsHeaderClient.h dnsHeaderHeader.h querry.h rData.h responseRecord.h
 

all		:

	$(CC) $(HEADERS) $(CFLAGS) $(NAME) $(CPP)

clean	:	

	rm -r $(OBJECT)
