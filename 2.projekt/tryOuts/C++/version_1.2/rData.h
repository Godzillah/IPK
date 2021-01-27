/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 5.4.2018 - 15:34
 *  NAME   : rData.h
 */

#ifndef PROJECT_RDATA_H
#define PROJECT_RDATA_H

#include "dnsHeader.h" // for unsigned short...
#include <stdint.h>    // for int32_t

typedef unsigned int u_int;

//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
    u_short type;               // type = A , AAAA , PTR , NS , CNAME
    u_short _class;             // class = IN | ....
    int32_t ttl;                // time to live
    u_short dataLen;           // data length
};
#pragma pack(pop)

class RData{

public:
        RData();            // constructor

        R_DATA r_data;

        // setters
        void setType(u_short);
        void set_Class(u_short);
        void setTtl(int32_t);
        void setDataLen(u_short);

        // getters
        u_short getType();
        u_short get_Class();
        int32_t getTtl();
        u_short getDataLen();

        // to get whole structure

        R_DATA getRData();

        virtual ~RData() ;  // destructor
private:

};

#endif //PROJECT_RDATA_H
