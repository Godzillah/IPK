/*	AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 5.4.2018 - 16:57
 *  NAME   : responseRecord.h
 */


#ifndef PROJECT_RESPONSERECORD_H
#define PROJECT_RESPONSERECORD_H

#include "dnsHeader.h"  // for typedef unsigned char...
#include "rData.h"      // resource
#include <stdint.h>    // for int32_t


//Types of DNS resource records
#define T_A 1       // Ipv4 address
#define T_NS 2      // Nameserver
#define T_CNAME 5   // Canonical name
#define T_PTR 12    // domain name pointer
#define T_AAAA 28   // Ipv6 address


//Pointers to resource record contents
struct RES_RECORD
{
    u_char *name;                   // name of response record
    struct R_DATA resource;         // structure R_DATA
    u_char *rData;
};

class ResponseRecord{


public:
    // constructor
    ResponseRecord();

    struct RES_RECORD res_record;


    // setters
    void setName(u_char* parName);
    void setRData(u_char* parRData);
    void setRdataType(u_short);
    void setRdataDatalen(u_short);
    void setRdataTtl(int32_t);
    void setRdata_Class(u_short);

    // getters
    u_char* getName();
    u_char* getRData();
    u_short getRDataType();
    u_short getRData_Class();
    int32_t getRdataTtl();
    u_short getRdataDataLen();

    // destructor
    virtual ~ResponseRecord();

private:


};






#endif //PROJECT_RESPONSERECORD_H
