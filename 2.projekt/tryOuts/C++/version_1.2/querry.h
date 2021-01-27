/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 20.3.2018 - 14:54
 *  NAME   : Querry.h
 */
#ifndef PROJECT_QUERRY_H
#define PROJECT_QUERRY_H

#include "dnsHeader.h" // for typedef u_char

struct QUESTION_STRUCT_STRUCT{
    u_short qtype;
    u_short qclass;
};

//Structure of a Query
typedef struct
{
    u_char *qname;
    struct QUESTION_STRUCT *ques;
} QUERY_STRUCT;

class Querry{

public:

    Querry();           // constructor

    struct QUESTION_STRUCT_STRUCT QUESTION_STRUCT;
    QUERY_STRUCT query;

    // setters
    void setQtype(u_short);
    void setQName(u_char*);
    void setQclass(u_short);

    // getters
    u_short getQtype();
    u_char* getQName();
    u_short getQclass();

    virtual ~Querry();   // destructor

private:
protected:
};



#endif //PROJECT_QUERRY_H
