/*
 *	AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 20.3.2018 - 23:33
 *  NAME   : ParseArgs.h
 */


#ifndef PARSEARGS_H
#define PARSEARGS_H

#include <string>
#include <string.h>
#include <iostream>
#include <regex>        // for ipv6

#define A 1       // ipv4 adrress
#define NS 2      // nameservers
#define CNAME 5   // canonical name
#define PTR 12    // domain PTR
#define AAAA 28   // ipv6

using namespace std;

class ParseArgs {
public:
    ParseArgs(int argc, char **argv);      	// konstruktor
    char* ip4Address;                       // -s povinný parametr, DNS server (IPv4 adresa), na který se budou odesílat dotazy.
    int timeOut;                            // -T volitelný parametr, timeout (v sekundách) pro dotaz
    int type;                               // -t volitelný parametr, typ dotazovaného záznamu: A (výchozí), AAAA, NS, PTR, CNAME
    char* domainName;					    // name povinny pamameter domena v pripade -t PTR na vstupe sa ocakava ipv4 alebo ipv6...

    virtual ~ParseArgs();                   //destruktor
private:
	void help();							// deklaracia napovedy

protected:

};

#endif /* PARSEARGS */