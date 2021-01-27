/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 4.3.2018 - 12:48
 *  NAME   : dnsHeader.h
 */


#ifndef SCHOOL_DNSHEADER_H
#define SCHOOL_DNSHEADER_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h> // for struct timeval

struct DnsHeader
{
    u_short id; // identification number

    u_char rd :1; // recursion desired
    u_char tc :1; // truncated message
    u_char aa :1; // authoritive answer
    u_char opcode :4; // purpose of message
    u_char qr :1; // query/response flag

    u_char rcode :4; // response code
    u_char cd :1; // checking disabled
    u_char ad :1; // authenticated data
    u_char z :1; // its z! reserved
    u_char ra :1; // recursion available

    u_short qCount; // number of question entries
    u_short answCount; // number of answer entries
    u_short authoCount; // number of authority entries
    u_short addiCount; // number of resource entries
};



#endif //SCHOOL_DNSHEADER_H
