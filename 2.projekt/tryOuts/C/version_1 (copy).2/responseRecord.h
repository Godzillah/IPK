/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 4.3.2018 - 12:48
 *  NAME   : dnsClient.h
 */


#ifndef SCHOOL_RESPONSERECORD_H
#define SCHOOL_RESPONSERECORD_H

#include "dnsClient.h"              // for u_char


//1  1  1  1  1  1
//0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                                               |
//|                                               |                 <---- THIS ONE
//|                      NAME                     |
//|                                               |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                      TYPE                     |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                     CLASS                     |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                      TTL                      |
//|                                               |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                   RDLENGTH                    |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
//|                     RDATA                     |                 <----  THIS ONE
//|                                               |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+



struct RESPONSE_RECORD
{
    u_char *cName;
    struct RESOURCE_DATA *resourceData;
    u_char *rData;
};

#endif //SCHOOL_RESPONSERECORD_H
