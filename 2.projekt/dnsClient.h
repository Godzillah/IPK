/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 4.3.2018 - 12:48
 *  NAME   : dnsClient.h
 */


#ifndef SCHOOL_DNSCLIENT_H
#define SCHOOL_DNSCLIENT_H


typedef unsigned short u_short;
typedef unsigned char u_char;
typedef unsigned int u_int;


#include <sys/socket.h>
#include <arpa/inet.h>          // for inet_ntoa ntohs and alos inet_addr...
#include <netinet/in.h>

#include "err.h"
#include "question.h"
#include "responseRecord.h"
#include "dnsHeader.h"
#include "rData.h"
#include "parseArgs.h"


void getHostByName (u_char* , int, int , char**, int);
void changeHostNameToDnsFormat (u_char*,u_char*);
u_char* readDnsFormat (u_char*,u_char*,int*);
double getTime();


#endif //SCHOOL_DNSCLIENT_H
