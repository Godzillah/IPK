/* 
 * File:   Sender.cpp
 * Author: matus
 * 
 * Created on Utorok, 2017, apríla 18, 18:40
 */

#include <string.h>

#include "Sender.h"


// konstruktor
// spracovanie IP adresy, vytvorenie socketu
Sender::Sender(const char *ip_address, int port) {
    
    struct addrinfo hints;
    struct sockaddr_in *h4;
    struct sockaddr_in6 *h6;
    int on = 1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((getaddrinfo(ip_address , to_string(port).c_str() , &hints , &server)) != 0)
        throw Error("Zla IP adresa.");


    if(server->ai_family == AF_INET){   // ak sa jedna o IPv4 adresu
        h4 = (struct sockaddr_in *) server->ai_addr;
        inet_ntop(server->ai_family, &(h4->sin_addr), ipv4, INET_ADDRSTRLEN); //z binarnej na textovu formu
        //ipVersion = 4;
        
        if (( socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
            throw Error("Chyba pri vytvarani socketu.");

        memset((char *) &localIPv4, 0, sizeof(localIPv4));
        memset((char *) &serverIPv4, 0, sizeof(serverIPv4));

        localIPv4.sin_family = AF_INET;
        localIPv4.sin_port = htons( port );

        serverIPv4.sin_family = AF_INET;
        serverIPv4.sin_port = htons( port );
        serverIPv4.sin_addr.s_addr = inet_addr( ipv4 );
        
        inet_pton(AF_INET, ipv4 , &serverIPv4.sin_addr.s_addr);
        
        
        if (setsockopt(socketFileDescriptor, SOL_IP, IP_RECVERR, &on, sizeof(on)))
            throw Error("Chyba pri nastovavani vlastnosti socketu.");
        
    }
    else if(server->ai_family == AF_INET6){   // ak sa jedna o IPv6 adresu
        h6 = (struct sockaddr_in6 *) server->ai_addr;
        inet_ntop(server->ai_family, &(h6->sin6_addr), ipv6, INET6_ADDRSTRLEN); //z binarnej na textovu formu
        //ipVersion = 6;
        
        if ((socketFileDescriptor = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
            throw Error("Chyba pri vytvarani socketu.");
        
        
        
        memset((char *) &localIPv6, 0, sizeof(localIPv6));
        memset((char *) &serverIPv6, 0, sizeof(serverIPv6));

        localIPv6.sin6_family = AF_INET6;
        localIPv6.sin6_port = htons( port );
        
        serverIPv6.sin6_family = AF_INET6;
        serverIPv6.sin6_port = htons( port );
        inet_pton(AF_INET6, ipv6, &serverIPv6.sin6_addr.s6_addr);

        inet_pton(AF_INET6, ipv6, &serverIPv6.sin6_addr.s6_addr);
        
        if (setsockopt(socketFileDescriptor, SOL_IPV6, IPV6_RECVERR, &on, sizeof(on)))
                throw Error("Chyba pri nastovavani vlastnosti socketu.");
    }
    
    if(server == NULL)
        throw Error("Zle zadana IP adresa.");
    


}

// posiela paket.
// aktivne cakanie kym nedorazi odpoved alebo nevyprsi cas 2 sekundy
bool Sender::send(int ttl){
    struct iovec iov;
    struct msghdr msg;
    struct cmsghdr *cmsg;
    struct icmphdr recvh;
    char buf[512];
    iov.iov_base = &recvh;
    iov.iov_len = sizeof(recvh);
    string sendedMsg = "huh?";
    bool hostnameShow = false;

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_flags = 0;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    
    if(server->ai_family == AF_INET){ 
        msg.msg_name = &localIPv4;
        msg.msg_namelen = sizeof(localIPv4);
        // nastavenia TTL
        if ( setsockopt( socketFileDescriptor, IPPROTO_IP, IP_TTL, (const char*)&ttl, sizeof( ttl ) ) )
            throw Error("Chyba pri nastovavani vlastnosti socketu.");
        
        if(sendto(socketFileDescriptor, sendedMsg.c_str(), sendedMsg.length(), 0, (struct sockaddr *)&serverIPv4, sizeof(serverIPv4))<0){
            if(sendto(socketFileDescriptor, sendedMsg.c_str(), sendedMsg.length(), 0, (struct sockaddr *)&serverIPv4, sizeof(serverIPv4))<0)
                throw Error("Chyba pri odosielani spravy");
        }
        
        
    } else if(server->ai_family == AF_INET6){
        msg.msg_name = &localIPv6;
        msg.msg_namelen = sizeof(localIPv6);
        if ( setsockopt( socketFileDescriptor, IPPROTO_IPV6, IPV6_UNICAST_HOPS, (const char*)&ttl, sizeof( ttl ) ) )
            throw Error("Chyba pri nastovavani vlastnosti socketu.");
        
        if(sendto(socketFileDescriptor,  sendedMsg.c_str(), sendedMsg.length(), 0, (struct sockaddr *)&serverIPv6, sizeof(serverIPv6))<0){
            if(sendto(socketFileDescriptor,  sendedMsg.c_str(), sendedMsg.length(), 0, (struct sockaddr *)&serverIPv6, sizeof(serverIPv6))<0)
                throw Error("Chyba pri odosielani spravy" );
        }
    }
    
    double sendTime = getTime();
    if( ttl < 10)
        cout << " " << ttl << "  " << flush;
    else
        cout << ttl << "  " << flush;
    int sendedRequests = 1;
    while(true){
        double elapsedtime = getTime() - sendTime;
        ssize_t bytesReceived = recvmsg(socketFileDescriptor, &msg, MSG_ERRQUEUE);
        if (bytesReceived < 0) {
            if(elapsedtime > 2000){
                if(sendedRequests < 3){
                    cout << "* " << flush;
                    sendTime = sendAnotherReq();
                    sendedRequests++;
                } else{
                    cout << "*" << endl << flush;
                    break;
                }
            }
            continue;
        }

        for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg)){
            struct sock_extended_err *e = (struct sock_extended_err*) CMSG_DATA(cmsg);
            
            if (cmsg->cmsg_level == SOL_IP && cmsg->cmsg_type == IP_RECVERR){              
                struct sockaddr_in *sin = (struct sockaddr_in *)(e+1);
                char ipAddres[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &sin->sin_addr, ipAddres, INET_ADDRSTRLEN);
                char *hostName = DNSlookup(ipAddres);
                string time = to_string(elapsedtime);
                int pos = time.find(".");
                time.erase( pos + 4 , time.length() - pos - 4);
                string timeOrError = time + " ms  ";
                
                if(hostnameShow == false){
                    cout << hostName << " (" << ipAddres << ")   " << flush;
                    hostnameShow = true;
                }
                
                switch(e->ee_type){
                    case ICMP_TIME_EXCEEDED:
                        cout << timeOrError << flush;
                        break;
                    case ICMP_DEST_UNREACH:
                        if(e->ee_code == 0){
                            timeOrError = "!N\0";
                        }
                        else if(e->ee_code == 1){
                            timeOrError = "!H\0";
                        }
                        else if(e->ee_code == 2){
                            timeOrError = "!P\0";
                        }
                        else if(e->ee_code == 13){
                            timeOrError = "!X\0";
                        }
                        cout << timeOrError << flush;
                        return true;
                }
            }
            else if(cmsg->cmsg_level == SOL_IPV6 && cmsg->cmsg_type == IPV6_RECVERR){
                
                if (e && e->ee_origin == SO_EE_ORIGIN_ICMP6){
                    struct sockaddr_in6 *sin = (struct sockaddr_in6 *)(e+1);
                    char ipAddres[INET6_ADDRSTRLEN];
                    inet_ntop(AF_INET6, &sin->sin6_addr, ipAddres, INET6_ADDRSTRLEN);
                    char *hostName = DNSlookup(ipAddres);
                    string time = to_string(elapsedtime);
                    int pos = time.find(".");
                    time.erase( pos , time.length() - pos);
                    string timeOrError = time + " ms  ";
                    if(hostName == NULL)
                        hostName = ipAddres;
                    
                    if(hostnameShow == false){
                        cout << hostName << " (" << ipAddres << ")   " << flush;
                        hostnameShow = true;
                    }
                    switch(e->ee_type){
                        case 3:
                            cout << timeOrError << flush;
                            break;
                        case 1:
                            if(e->ee_code == 6){ //reject route to dest
                                timeOrError = "!N\0";
                            }
                            else if(e->ee_code == 3){ // adress unreach
                                timeOrError = "!H\0";
                            }
                            else if(e->ee_code == 1){ //comm admin prohibited
                                timeOrError = "!X\0";
                            }
                            cout << timeOrError << flush;
                            return true;
                    }
                }
            }
            
        }
        if( sendedRequests < 3){
            sendTime = sendAnotherReq();
            sendedRequests++;
        }
        else {
            cout << endl << flush;
            break;
        }
    }
    return false;


}


// posle dalsi paket a vrati cas odoslania
double Sender::sendAnotherReq(){
    string sendedMsg = "huh?";
    if(server->ai_family == AF_INET){ 
        if(sendto(socketFileDescriptor, sendedMsg.c_str(), sendedMsg.length(), 0, (struct sockaddr *)&serverIPv4, sizeof(serverIPv4))<0){
            if(sendto(socketFileDescriptor, sendedMsg.c_str(), sendedMsg.length(), 0, (struct sockaddr *)&serverIPv4, sizeof(serverIPv4))<0)
                throw Error("Chyba pri odosielani spravy.");
        }
    }  else if(server->ai_family == AF_INET6){
        if(sendto(socketFileDescriptor,  sendedMsg.c_str(), sendedMsg.length(), 0, (struct sockaddr *)&serverIPv6, sizeof(serverIPv6))<0){
            if(sendto(socketFileDescriptor,  sendedMsg.c_str(), sendedMsg.length(), 0, (struct sockaddr *)&serverIPv6, sizeof(serverIPv6))<0)
                throw Error("Chyba pri odosielani spravy." );
        }
    }
    return getTime();
}


// reverzne hladanie hostname podla IP adresy
char *Sender::DNSlookup(char *ip){ //should work for both v4 and v6
    struct addrinfo *result;
    struct addrinfo *res;
    int error;
    /* resolve the domain name into a list of addresses */
    error = getaddrinfo(ip, NULL, NULL, &result);
    if (error != 0){
        freeaddrinfo(result);
        return NULL;
    }
    /* loop over all returned results and do inverse lookup */
    for (res = result; res != NULL; res = res->ai_next)
    {
        char *hostname = (char *)calloc(NI_MAXHOST, NI_MAXHOST * sizeof(char));

        error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
        if (error != 0)
            continue;

        if (strcmp(hostname, "\0") != 0){
            freeaddrinfo(result);
            return hostname;
        }
        free(hostname);
    }
    freeaddrinfo(result);
    return NULL;

}

//vracia aktualny cas v milisekundach
double Sender::getTime(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (((double) tv.tv_usec) / 1000. + (unsigned long) tv.tv_sec * 1000) ;

}


Sender::~Sender() {
}

