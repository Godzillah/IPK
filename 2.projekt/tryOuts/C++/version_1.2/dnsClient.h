/*	AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 6.4.2018 - 14:14
 *  NAME   : dnsHeaderHeader.h
 */

#ifndef PROJECT_DNSCLIENT_H
#define PROJECT_DNSCLIENT_H

//#include "dnsHeader.h"
//#include "responseRecord.h"
//#include "rData.h"
//#include "querry.h"
#include "err.h"


// -- SKUSANIE LIBRARIES --
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

#include <netinet/in.h>     // sockaddr_in .....  in_addr
#include <netdb.h>
#include <ctype.h>
// -- SKUSANIE LIBRARIES -- POTOM ODSTRAN


#define T_A 1 //Ipv4 address
#define T_NS 2 //Nameserver
#define T_CNAME 5 // canonical name
#define T_SOA 6 /* start of authority zone */
#define T_PTR 12 /* domain name pointer */
#define T_MX 15 //Mail server

class DnsClient{

public:

    DnsClient();           // constructor

    //List of DNS Servers registered on the system
    char dnsHeader_servers[10][100];
    int dnsHeaderServerCount = 0;

    void getHostByName(unsigned char *host, int querryType,int argc , char** argv);       // TODO: implement this method
    u_char* readName(unsigned char* reader , unsigned char* buffer , int* count);   // TODO: implement this method
    void changeToDnsNameFormat(unsigned char* dnsHeader, unsigned char* host);


    virtual ~DnsClient();   // destructor

private:

    // composition
 //   DnsHeader dnsHeader;
 //   ResponseRecord responseRecord;
 //   RData rData;
 //   Querry querryInfo;

protected:


};






#endif //PROJECT_DNSCLIENT_H
