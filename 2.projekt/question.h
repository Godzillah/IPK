/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 4.3.2018 - 12:48
 *  NAME   : question.h
 */


#ifndef SCHOOL_QUESTION_STRUCT_H
#define SCHOOL_QUESTION_STRUCT_H

#include "dnsClient.h"      // for u_short


//1  1  1  1  1  1
//0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                                               |
//|                     QNAME                     |
//|                                               |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                     QTYPE                     |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//|                     QCLASS                    |
//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

struct QUESTION_STRUCT
{
    u_short qtype;
    u_short qclass;
};


typedef struct
{
    u_char *cName;
    struct QUESTION_STRUCT *ques;
} QUERY_STRUCT;

#endif //SCHOOL_QUESTION_STRUCT_H
