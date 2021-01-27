 /* 
 * FILE  : gethostbyname.cpp
 * AUTHOR: Maros Orsak
 * LOGIN : xorsak02
 * DATE  : 30.3.2018
 */

#include "gethostbyname.h"
using namespace std;

/**
 * Constructor definition
 */

Gethostbyname::Gethostbyname(){
	printf("Constructor called...\n");
}


void Gethostbyname::convertHostNameToIp(hostent *host_adrress , in_addr **addr_list) {

	host_adrress = gethostbyname("merlin.fit.vutbr.cz");
	if(host_adrress == NULL){
		// SEM BY BOL INAKSIE THROW ERROR ))
		printf("SHIT HAPPENS\n");
		exit(1);
	}

	addr_list = (struct in_addr**) host_adrress->h_addr_list;

	for(i = 0 ; addr_list[i] != NULL; i++){
		strcpy(ip, inet_ntoa(*addr_list[i]));
	}

	 printf("|Toto je HOSTNAME : %s|\n|Toto je IP adresa : %s|\n" , host_adrress->h_name , ip);
}


void Gethostbyname::run(){

	// calling method....
	convertHostNameToIp(host_adrress,addr_list);
}

/**
 * Destructor definition 
 */
 Gethostbyname::~Gethostbyname(){
 	printf("Destructor called...\n");
 }
