/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 4.3.2018 - 12:48
 *  NAME   : dnsClient.cpp
 */

#include "dnsClient.h"
#include "err.h"
#include "dnsHeader.h"

/**
 * @brief funcktion which inicialize data for Dns Header.
 * @param dnsHeader
 */

void initDnsHeader(struct DnsHeader *dnsHeader){


//    1  1  1  1  1  1
//    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
//    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//    |                      ID                       |
//    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
//    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//    |                    QDCOUNT                    |
//    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//    |                    ANCOUNT                    |
//    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//    |                    NSCOUNT                    |
//    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//    |                    ARCOUNT                    |
//    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+


    // FIRST 16-bits
    dnsHeader->id = (u_short) htons(getpid());          // identification number

    // SECOND 16-bits
    //************************
    // 1-bit
    dnsHeader->qr = 0;              // query/response flag
    // 4-bits
    dnsHeader->opcode = 0;          // purpose of message
    // 1-bit
    dnsHeader->aa = 0;              // authoritive answer
    // 1-bit
    dnsHeader->tc = 0;              // truncated message
    // 1-bit
    dnsHeader->rd = 1;              // recursion desired
    // 1-bit
    dnsHeader->ra = 0;              // recursion available
    // 1-bits
    dnsHeader->z = 0;               // z reserved for future used
    // 1-bit
    dnsHeader->ad = 0;              // authenticated data
    // 1-bit
    dnsHeader->cd = 0;              // checking disabled


    //************************

    // 16-bits
    dnsHeader->qCount = htons(1);   // number of question entries
    // 16-bits
    dnsHeader->answCount = 0;       // number of answer entries
    // 16-bits
    dnsHeader->authoCount = 0;      // number of authority entries
    // 16-bits
    dnsHeader->addiCount = 0;       // number of resource entries

}

/**
 * @brief function which returns actual time
 * @return
 */

double getTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (((double) tv.tv_usec) / 1000. + (unsigned long) tv.tv_sec);
}

/**
 * @brief function which converts classic Domain to ipAdrress(forward lookup) also reverse lookup
 * @param hostName
 * @param queryType
 * @param argc
 * @param argv
 * @param ttl
 */
void getHostByName(u_char *hostName , int queryType ,int argc, char** argv,int ttl)
{
    /* deklarations of variables */
    // ---------------------------------

    u_char array[512];                                         // for UPD meessage transport
    u_char *querryName;
    u_char *readerPr;

    int number;
    int j;


    struct sockaddr_in myAddr;                                 // declaration of structure sockaddr_in
    struct sockaddr_in destination;                            // declaration of structure sockaddr_in

    struct sockaddr_in6 myAddr6;                               // declaration of structure for ipv6
    struct sockaddr_in6 destination6;                          // declaration of structure for ipv6


    struct DnsHeader *pDns = NULL;                             // inicialization of struckture
    struct QUESTION_STRUCT *querryInfo = NULL;                 // inicialization of structure
    struct RESPONSE_RECORD arrayOfAnswers[10];                 // answers of DNS server..

    // ----------------------------------

    // TODO  : sem sa bude vytvarat socket bud ipv4 alebo ipv6
    // crreating socket which bears  the information about connection between two devices
    int mySocket = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP); //UDP packet for DNS queries

    if (mySocket < 0){                  // <0 in case there was a problem creating it
        perror("Creating socket");
    }

    destination.sin_family = AF_INET;                           //same as with socket Special family protocol...
    destination.sin_port = htons(53);                           // port 53 is mainly used for DNS data flow
    destination.sin_addr.s_addr = inet_addr(argv[2]);           // we connect to the server from argument

    pDns = (struct DnsHeader *)&array;                           // setting the pDns structure to standard queries

    initDnsHeader(pDns);                                         // inicialization of header....

    querryName =(u_char*)&array[sizeof(struct DnsHeader)];      // need adress of array..
    changeHostNameToDnsFormat(querryName , hostName);               // fucntion which converting www.google.com -> 3www6google3com

    querryInfo =(struct QUESTION_STRUCT*)&array[sizeof(struct DnsHeader) + (strlen((const char*)querryName) + 1)]; //fill it

    querryInfo->qtype = htons( queryType );                     // type of query A AAAA PTR type/...
    querryInfo->qclass = htons(1);                              // class IN = internet


    struct timeval timeout;                     // structure to measure elapsed time
    timeout.tv_sec = ttl;
    timeout.tv_usec = ttl * 1000;

    double sendTime = getTime();                // remember act_time


    if ((setsockopt(mySocket, SOL_IP, IP_RECVERR, &timeout, sizeof(timeout))) < 0)  // setiing timeOut
        throw Err("Chyba pri nastovavani vlastnosti socketu.");

    // sending packet to the server
    if( sendto(mySocket,(char*)array,sizeof(struct QUESTION_STRUCT) + (strlen((const char*)querryName)+1) + sizeof(struct DnsHeader) ,0,(struct sockaddr*)&destination,sizeof(destination)) < 0)
    {
        perror("Sending message failed");
    }

    double elapsedtime = getTime() - sendTime;                                      // elapsing time... and condition...
    if(elapsedtime > ttl){
        throw Err("Spojenie ku serveru zlyhalo",1);
    }

    // receiving a packet from server

    number = sizeof destination;

    if(recvfrom (mySocket,(char*)array , 512 , 0 , (struct sockaddr*)&destination , (socklen_t*)&number ) < 0)
    {
        perror("Receiving messsage failed");
    }

    pDns = (struct DnsHeader*) array;

    //move ahead of the dns header and the query field
    readerPr = &array[sizeof(struct QUESTION_STRUCT) + (strlen((const char*)querryName)+1) + sizeof(struct DnsHeader) ];

    int startStop = 0;
    // reading answer arrays....

    for(number=0;number<ntohs(pDns->answCount);number++)
    {
        arrayOfAnswers[number].cName=readDnsFormat(readerPr,array,&startStop);
        readerPr = readerPr + startStop; // in the case there were offsets in previous precord

        arrayOfAnswers[number].resourceData = (struct RESOURCE_DATA*)(readerPr);
        readerPr = readerPr + sizeof(struct RESOURCE_DATA);  // moving next to
/* in case there is is flag fot type A , all of them all described higher this one for instance
 * is to type a record containing Ipv4 , it is considered to be the most common record */
        if(ntohs(arrayOfAnswers[number].resourceData->type) == A) //if its an ipv4 address
        {
            arrayOfAnswers[number].rData = (u_char*)malloc(ntohs(arrayOfAnswers[number].resourceData->dataLength));
            /*for cyclus to fill the answer of certain number */
            for(j=0 ; j<ntohs(arrayOfAnswers[number].resourceData->dataLength) ; j++)
            {
                arrayOfAnswers[number].rData[j]=readerPr[j];
            }

            arrayOfAnswers[number].rData[ntohs(arrayOfAnswers[number].resourceData->dataLength)] = '\0';
            readerPr = readerPr + ntohs(arrayOfAnswers[number].resourceData->dataLength);
        }
        else
        {
            arrayOfAnswers[number].rData = readDnsFormat(readerPr,array,&startStop);
            readerPr = readerPr + startStop;
        }   //structures above have all the same purpose and it is to transport records from arrays
            // to suitable structures for future print.
    }

    // printing answer arrays...
    for(number=0 ; number < ntohs(pDns->answCount) ; number++)
    {
        printf("%s ",arrayOfAnswers[number].cName);

        if( ntohs(arrayOfAnswers[number].resourceData->type) == A) //IPv4 address
        {
            long *temp;
            temp=(long*)arrayOfAnswers[number].rData;
            myAddr.sin_addr.s_addr=(*temp); //working without ntohl
            printf("IN A %s ",inet_ntoa(myAddr.sin_addr));
        }

        if(ntohs(arrayOfAnswers[number].resourceData->type)== CNAME)
        {
            //Canonical Nmame for an alias
            printf("IN CNAME %s",arrayOfAnswers[number].rData);
        }
        if(ntohs(arrayOfAnswers[number].resourceData->type)== NS)
        {
            //nameserver
            printf("IN NS %s",arrayOfAnswers[number].rData);
        }
                //adress to get the name using arpa
        if(ntohs(arrayOfAnswers[number].resourceData->type == PTR))
        {
            // TODO: if i got more time :(
            //ptr....
            printf("IN PTR %s",arrayOfAnswers[number].rData);

        }
        printf("\n");
    }

    return;
}

/**
 * @brief function which reading 3www6google3com(dnsFormat) format
 * @param readerPr
 * @param arrayfer
 * @param count
 * @return  domainAndDnsName
 */

u_char* readDnsFormat(u_char* readerPr,u_char* array,int* counter)
{
    u_char *domainAndDnsName;

    u_int jumpOn=0;                                 // helpful flag which tell when i jump or on to other location
    u_int offSet;                                   // we will use it to locate place of pointer in buffer

    domainAndDnsName = (u_char*)malloc(256);        // 8 bits..
    domainAndDnsName[0]='\0';                       // memset buffer.....

    *counter = 1;
    u_int strComp = 0;                              //string completed

    for (; *readerPr != 0 ; readerPr = readerPr + 1)
    {
        if(*readerPr < 192) //255-63 = menas that if this number is lower than 192 one od subdomen
        {//is given by reference, there is a function build in our DNS system to see how it works
            domainAndDnsName[strComp++]=*readerPr;
        }
        else
        {
//            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//            | 1  1|                OFFSET                   |     < -- for 49152
//            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
/* offSet represent the way how protocol in DNS saves bites , for when we think about space in buffer
 * and domein name strucutre we will soon realize there is space of size 256 bytes which gives us
 * 64, minus one what is byte for dot or using compression numbers, gives 63 what is also the max size
 * but what is most important using offset means in binary 1100000 0000000 and it gives 49152 in decadic
 * once we count down the difference there are just a few bites whose purpose is to give us the
 * location of first apperance of suffix in previous part  */

            offSet = 256*(*readerPr) + *(readerPr+1) - 49152;
            readerPr = array + offSet - 1;
            jumpOn = 1;         // jumpping on other location not up counter
        }

        if(jumpOn==0) {         // means we continue beside in case there was no jump
            *counter = *counter + 1; //if do not jump on other location up counter
        }
    }
    if(jumpOn==1)
    {
        *counter = *counter + 1; // number of moved steps in packet
    }

    domainAndDnsName[strComp]='\0';
    // converting for example --- > 3www8facebook3com0 www.facebook.com

    int i , j;

    for( i = 0 ; (int)strlen((const char*)domainAndDnsName) > i ; i++ )     // iterating of length domainAndDnsName
    {
        strComp = domainAndDnsName[i];
        for( j=0; (int)strComp > j ; j++)
        {
            domainAndDnsName[i] = domainAndDnsName[i+1]; //continue writing on place of first dot ...
            i=i+1;
        }
        domainAndDnsName[i]='.';
    }

    domainAndDnsName[i-1]='\0'; // for detail removing last dot . ? which symbols root...

    return domainAndDnsName;
}

 /**
  * @brief fucntion which converting www.google.com -> 3www6google3com
  * @param pDns
  * @param hostName
  */

void changeHostNameToDnsFormat(u_char* pDns,u_char* hostName)
{
    u_int position = 0;                                     // inicialization postion
    u_int index = 0;                                        // declaration of index
    strcat((char*)hostName,".");                            // end the end of string put .

        while (index < strlen((char *) hostName)) {         // iterate to length hostName
            if (hostName[index] == '.') {                   // if we find .
                *pDns++ = index - position;                 // array which helps us parsing parts of hostName
                while(position < index){                    // iterate if we find that .
                    *pDns++ = hostName[position];
                    position++;                             // inc position
                }
                position++;                                 // inc position
            }
            index++;                                        // inc index
        }

    *pDns = '\0';                                           // clean array
}