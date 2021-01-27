/*	AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 6.4.2018 - 14:12
 *  NAME   : dnsHeaderHeader.cpp
 */

#include "dnsClient.h"
#include "parseArgs.h"


DnsClient::DnsClient() {
    cout << "Starting DnsClient..." << endl;
}



//DNS header structure
struct DnsHeader
{
    unsigned short id; // identification number

    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag

    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // its z! reserved
    unsigned char ra :1; // recursion available

    unsigned short q_count; // number of QUESTION_STRUCT entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};

//Constant sized fields of query structure
struct QUESTION_STRUCT
{
    unsigned short qtype;
    unsigned short qclass;
};

//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
    unsigned short type;
    unsigned short _class;
    unsigned int ttl;
    unsigned short data_len;
};
#pragma pack(pop)

//Pointers to resource record contents
struct RES_RECORD
{
    unsigned char *name;
    struct R_DATA *resource;
    unsigned char *rdata;
};

//Structure of a Query
typedef struct
{
    unsigned char *name;
    struct QUESTION_STRUCT *ques;
} QUERY;


/**
 * @brief Function change for format www.google.com to 3www6google3com etc.
 * @param dnsHeader
 * @param host
 */

void DnsClient::changeToDnsNameFormat(unsigned char *dnsHeader, unsigned char *host) {


    unsigned int lock = 0;
    unsigned int i;

    strcat((char*)host,".");

    for(i = 0 ; i < strlen((char*)host) ; i++){
        if(host[i] == '.'){
            *dns++ = i - lock;
            for(;lock<i;lock++){
                *dns++=host[lock];
            }
            lock++;
        }
    }
    *dns++ = '\0';
}


u_char* DnsClient::readName(unsigned char* reader,unsigned char* buffer,int* count) {
    unsigned char *name;
    unsigned int p=0,jumped=0,offset;
    int i , j;

    *count = 1;
    name = (unsigned char*)malloc(256);

    name[0]='\0';

    //read the names in 3www6google3com format
    while(*reader!=0)
    {
        //
        if(*reader>=192)
        {
            offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000 ;)
            reader = buffer + offset - 1;
            jumped = 1; //we have jumped to another location so counting wont go up!
        }
        else
        {
            name[p++]=*reader;
        }

        reader = reader+1;

        if(jumped==0)
        {
            *count = *count + 1; //if we havent jumped to another location then we can count up
        }
    }

    name[p]='\0'; //string complete
    if(jumped==1)
    {
        *count = *count + 1; //number of steps we actually moved forward in the packet
    }

    //now convert 3www6google3com0 to www.google.com
    for(i=0;i<(int)strlen((const char*)name);i++)
    {
        p=name[i];
        for(j=0;j<(int)p;j++)
        {
            name[i]=name[i+1];
            i=i+1;
        }
        name[i]='.';
    }
    name[i-1]='.'; //remove the last dot
    return name;
}

 // Perform a DNS query by sending a packet

void DnsClient::getHostByName(unsigned char *host , int queryType, int argc , char** argv)
{
    unsigned char buf[65536],*qname,*reader;
    int i , j , stop , s;

    struct sockaddr_in a;

    struct RES_RECORD arrayOfarrayOfAnswers[20],auth[20],addit[20]; //the replies from the DNS server
    struct sockaddr_in destination;

    struct DnsHeader *dns = NULL;
    struct QUESTION_STRUCT *qinfo = NULL;

    printf("Resolving %s" , host);

    s = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP); //UDP packet for DNS queries

    destination.sin_family = AF_INET;
    destination.sin_port = htons(53);
    destination.sin_addr.s_addr = inet_addr(argv[2]); //dns servers

    //Set the DNS structure to standard queries
    dns = (struct DnsHeader *)&buf;

    dns->id = (unsigned short) htons(getpid());
    dns->qr = 0; //This is a query
    dns->opcode = 0; //This is a standard query
    dns->aa = 0; //Not Authoritative
    dns->tc = 0; //This message is not truncated
    dns->rd = 1; //Recursion Desired
    dns->ra = 0; //Recursion not available! hey we dont have it (lol)
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1); //we have only 1 QUESTION_STRUCT
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;

    //point to the query portion
    qname =(unsigned char*)&buf[sizeof(struct DnsHeader)];

    printf("\nThis is qname:%s" ,qname );



    changeToDnsNameFormat(qname , host);
    qinfo =(struct QUESTION_STRUCT*)&buf[sizeof(struct DnsHeader) + (strlen((const char*)qname) + 1)]; //fill it

    qinfo->qtype = htons( queryType ); //type of the query , A , MX , CNAME , NS etc
    qinfo->qclass = htons(1); //its internet (lol)

    printf("\nSending Packet...");
    if( sendto(s,(char*)buf,sizeof(struct DnsHeader) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION_STRUCT),0,(struct sockaddr*)&destination,sizeof(destination)) < 0)
    {
        perror("sendto failed");
    }
    printf("Done");

    //Receive the answer
    i = sizeof destination;
    printf("\nReceiving answer...");
    if(recvfrom (s,(char*)buf , 65536 , 0 , (struct sockaddr*)&destination , (socklen_t*)&i ) < 0)
    {
        perror("recvfrom failed");
    }
    printf("Done");

    dns = (struct DnsHeader*) buf;

    //move ahead of the dns header and the query field
    reader = &buf[sizeof(struct DnsHeader) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION_STRUCT)];

    printf("\nThe response contains : ");
    printf("\n %d QUESTION_STRUCTs.",ntohs(dns->q_count));
    printf("\n %d Answers.",ntohs(dns->ans_count));
    printf("\n %d Authoritative Servers.",ntohs(dns->auth_count));
    printf("\n %d Additional records.\n\n",ntohs(dns->add_count));

    //Start reading arrayOfAnswers
    stop=0;

    for(i=0;i<ntohs(dns->ans_count);i++)
    {
        arrayOfAnswers[i].name=readName(reader,buf,&stop);
        reader = reader + stop;

        arrayOfAnswers[i].resource = (struct R_DATA*)(reader);
        reader = reader + sizeof(struct R_DATA);

        if(ntohs(arrayOfAnswers[i].resource->type) == 1) //if its an ipv4 address
        {
            arrayOfAnswers[i].rdata = (unsigned char*)malloc(ntohs(arrayOfAnswers[i].resource->data_len));

            for(j=0 ; j<ntohs(arrayOfAnswers[i].resource->data_len) ; j++)
            {
                arrayOfAnswers[i].rdata[j]=reader[j];
            }

            arrayOfAnswers[i].rdata[ntohs(arrayOfAnswers[i].resource->data_len)] = '\0';

            reader = reader + ntohs(arrayOfAnswers[i].resource->data_len);
        }
        else
        {
            arrayOfAnswers[i].rdata = readName(reader,buf,&stop);
            reader = reader + stop;
        }
    }

    //read authorities
    for(i=0;i<ntohs(dns->auth_count);i++)
    {
        auth[i].name=readName(reader,buf,&stop);
        reader+=stop;

        auth[i].resource=(struct R_DATA*)(reader);
        reader+=sizeof(struct R_DATA);

        auth[i].rdata=readName(reader,buf,&stop);
        reader+=stop;
    }

    //read additional
    for(i=0;i<ntohs(dns->add_count);i++)
    {
        addit[i].name=readName(reader,buf,&stop);
        reader+=stop;

        addit[i].resource=(struct R_DATA*)(reader);
        reader+=sizeof(struct R_DATA);

        if(ntohs(addit[i].resource->type)==1)
        {
            addit[i].rdata = (unsigned char*)malloc(ntohs(addit[i].resource->data_len));
            for(j=0;j<ntohs(addit[i].resource->data_len);j++)
                addit[i].rdata[j]=reader[j];

            addit[i].rdata[ntohs(addit[i].resource->data_len)]='\0';
            reader+=ntohs(addit[i].resource->data_len);
        }
        else
        {
            addit[i].rdata=readName(reader,buf,&stop);
            reader+=stop;
        }
    }

    //print arrayOfAnswers
    printf("\nAnswer Records : %d \n" , ntohs(dns->ans_count) );
    for(i=0 ; i < ntohs(dns->ans_count) ; i++)
    {
        printf("Name : %s ",arrayOfAnswers[i].name);

        if( ntohs(arrayOfAnswers[i].resource->type) == T_A) //IPv4 address
        {
            long *p;
            p=(long*)arrayOfAnswers[i].rdata;
            a.sin_addr.s_addr=(*p); //working without ntohl
            printf("has IPv4 address : %s",inet_ntoa(a.sin_addr));
        }

        if(ntohs(arrayOfAnswers[i].resource->type)==5)
        {
            //Canonical name for an alias
            printf("has alias name : %s",arrayOfAnswers[i].rdata);
        }

        printf("\n");
    }

    //print authorities
    printf("\nAuthoritive Records : %d \n" , ntohs(dns->auth_count) );
    for( i=0 ; i < ntohs(dns->auth_count) ; i++)
    {

        printf("Name : %s ",auth[i].name);
        if(ntohs(auth[i].resource->type)==2)
        {
            printf("has nameserver : %s",auth[i].rdata);
        }
        printf("\n");
    }

    //print additional resource records
    printf("\nAdditional Records : %d \n" , ntohs(dns->add_count) );
    for(i=0; i < ntohs(dns->add_count) ; i++)
    {
        printf("Name : %s ",addit[i].name);
        if(ntohs(addit[i].resource->type)==1)
        {
            long *p;
            p=(long*)addit[i].rdata;
            a.sin_addr.s_addr=(*p);
            printf("has IPv4 address : %s",inet_ntoa(a.sin_addr));
        }
        printf("\n");
    }
    return;
}


DnsClient::~DnsClient() {

    cout << "Ending DnsClient..." << endl;
}



