#AUTHOR		: MAROS ORSAK
#BRIEF		: makefile...(subor pomocou ktoreho budeme moct prelozit , spustit program)

NAME		= ipk-lookup
CC 			= g++
CFLAGS 		= -Wextra -std=c++11 -o 
OBJECT		= ipk-lookup
CPP			= main.cpp parseArgs.cpp err.cpp dnsClient.cpp
HEADERS 	= err.h parseArgs.h responseRecord.h question.h dnsClient.h dnsHeader.h rData.h
 

all		:

	$(CC) $(HEADERS) $(CFLAGS) $(NAME) $(CPP)

clean	:	

	rm -r $(OBJECT)
