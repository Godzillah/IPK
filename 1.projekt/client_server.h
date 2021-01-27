
#ifndef _CLIENT_SERVER_H_
#define _CLIENT_SERVER_H_

// definicia makier ktore vyuzivam 

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// vsetky potrebne kniznice ktore su vyuzivane pre client aj server


#include <stdio.h>
#include <string.h>         
#include <stdlib.h>         
#include <stdbool.h>

#include <sys/socket.h>
#include <sys/stat.h>	   
#include <sys/types.h>

#include <arpa/inet.h>      //inet_addr
#include <unistd.h>         //write
#include <pthread.h>        //lpthread
#include <dirent.h>
#include <errno.h>

#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>

#include <pwd.h>

#endif

// pre command pwd 

#ifdef __unix__
    #define IS_POSIX 1
    #include <unistd.h>
#else
    #define IS_POSIX 0
#endif



