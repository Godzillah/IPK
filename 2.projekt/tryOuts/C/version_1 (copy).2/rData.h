/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 4.3.2018 - 12:48
 *  NAME   : rData.h
 */


#ifndef SCHOOL_RDATA_H
#define SCHOOL_RDATA_H

#include "dnsClient.h"  // for u_short

// the RR specific data whose length is defined by RDLENGTH, for instance, 192.168.254.2

//1  1  1  1  1  1
//0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                                               |
//|                                               |
//|                      NAME                     |
//|                                               |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                      TYPE                     |                 <---- THIS ONE
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                     CLASS                     |                 <---- THIS ONE
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                      TTL                      |                 <---- THIS ONE
//|                                               |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                   RDLENGTH                    |                 <---- THIS ONE
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
//|                     RDATA                     |
//|                                               |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

#pragma pack(push, 1)
struct RESOURCE_DATA
{
    u_short type;
    u_short _class;
    u_int ttl;
    u_short dataLength;
};
#pragma pack(pop)

#endif //SCHOOL_RDATA_H
