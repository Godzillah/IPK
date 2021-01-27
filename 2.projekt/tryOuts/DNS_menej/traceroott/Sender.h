/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sender.h
 * Author: matus
 *
 * Created on Utorok, 2017, apríla 18, 18:40
 */

#ifndef SENDER_H
#define SENDER_H



#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <math.h>  
#include <unistd.h>
#include <linux/errqueue.h>

#include "Error.h"

using namespace std;

class Sender {
public:
    Sender(const char *ip_address, int port);
    bool send( int ttl);
    virtual ~Sender();
private:
    double sendAnotherReq();
    double getTime();
    char *DNSlookup( char *ip);
    struct addrinfo *server;
    struct sockaddr_in localIPv4;
    struct sockaddr_in serverIPv4;
    struct sockaddr_in6 localIPv6;
    struct sockaddr_in6 serverIPv6;
    char ipv4[INET_ADDRSTRLEN];
    char ipv6[INET6_ADDRSTRLEN];
    int socketFileDescriptor;
};

#endif /* SENDER_H */

