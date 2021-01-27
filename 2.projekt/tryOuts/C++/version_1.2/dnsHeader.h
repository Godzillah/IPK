/*	AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 5.4.2018 - 2:16
 *  NAME   : dnsHeader.h
 */

#ifndef DNSHEADER_H
#define DNSHEADER_H

#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h> // for getpid()
#include <arpa/inet.h> // htons... etc.

using namespace std;

typedef unsigned short u_short;
typedef unsigned char u_char;

//DNS header structure
struct DnsHeader
{
    u_short id; // identification number

    u_char rd :1; // recursion desired      // TODO: takto alokujem 1 bajt ale ja musim jeden BIT !!
    u_char tc :1; // truncated message
    u_char aa :1; // authoritive answer
    u_char opcode :4; // purpose of message
    u_char qr :1; // query/response flag

    u_char rcode :4; // response code
    u_char cd :1; // checking disabled
    u_char ad :1; // authenticated data
    u_char z :1; // its z! reserved
    u_char ra :1; // recursion available

    u_short q_count; // number of QUESTION_STRUCT entries
    u_short ans_count; // number of answer entries
    u_short auth_count; // number of authority entries
    u_short add_count; // number of resource entries
};

class DnsHeader {
public:
    DnsHeader();      					    // constructor

    struct DnsHeader dnsHeader;

    void setHeader();


    virtual ~DnsHeader();                   // destructor

private:
protected:

};

#endif /* DNSHEADER */