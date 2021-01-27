/* 
 * FILE  : gethostbyname.h
 * AUTHOR: Maros Orsak
 * LOGIN : xorsak02
 * DATE  : 30.3.2018
 */


#ifndef _GETHOSTBYNAME_H
#define	_GETHOSTBYNAME_H

#include <stdio.h>
#include <string.h> 
#include <string>        
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

class Gethostbyname{

public:
	/**
	 * Constructor declaration
	 */
	Gethostbyname();

	/**
	 * Destructor declaration
	 */
	virtual ~Gethostbyname();	

	 /**
	 * Function that runs the gethostbyname fucntion
	 */
	void run();


private:

	struct hostent *host_adrress;
	struct in_addr **addr_list;
	char ip[50];
	int i;

	/**
	 * Function that converts Hostname to Ip adress 
	 * @param hostent return pointer to hostent stucture 
	 * @param in_addr structure
	 */
	void convertHostNameToIp(hostent *host_adrress , in_addr **addr_list);
	
	
};








#endif	/* _DNS_REQUEST_H */