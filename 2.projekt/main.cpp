/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 21.3.2018 - 10:48
 *  NAME   : main.cpp
 */

#include "dnsClient.h"


using namespace std;

int main( int argc , char *argv[])
{
    try {
        ParseArgs *args = new ParseArgs(argc , argv);
        getHostByName((u_char*)args->domainName ,args->type, argc , argv, args->timeOut );
        delete(args);
    }catch (Err &err){
        err.printExit();
    }
    return 0;
}


