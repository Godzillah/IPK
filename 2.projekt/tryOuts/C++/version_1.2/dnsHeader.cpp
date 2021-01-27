/*	AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 5.4.2018 - 4:07
 *  NAME   : dnsHeader.cpp
 */

#include "dnsHeader.h"

DnsHeader::DnsHeader() {
    //Set the DNS structure to standard queries
    cout << "DnsHeader was called..." << endl;

    this->dnsHeaderHeader.id = (unsigned short) htons(getpid()); // id
    this->dnsHeaderHeader.qr = 0;       // query response
    this->dnsHeaderHeader.opcode = 0;   // standard query
    this->dnsHeaderHeader.aa = 0;       // authoritative answer
    this->dnsHeaderHeader.tc = 0;       // truncation
    this->dnsHeaderHeader.rd = 1;       // recursion desired
    this->dnsHeaderHeader.ra = 0;       // recursion available
    this->dnsHeaderHeader.z = 0;        //
    this->dnsHeaderHeader.ad = 0;       //
    this->dnsHeaderHeader.cd = 0;       //
    this->dnsHeaderHeader.rcode = 0;    //

    this->dnsHeaderHeader.q_count = htons(1);   // we have only 1 QUESTION_STRUCT (QUESTION_STRUCT count)
    this->dnsHeaderHeader.ans_count = 0;        // answer record count
    this->dnsHeaderHeader.auth_count = 0;       // authoritative record count
    this->dnsHeaderHeader.add_count = 0;        // additional record count

}

void DnsHeader::setHeader() {

    cout << "Setting header..." << endl;
    this->dnsHeaderHeader.id = (unsigned short) htons(getpid()); // id
    this->dnsHeaderHeader.qr = 0;       // query response
    this->dnsHeaderHeader.opcode = 0;   // standard query
    this->dnsHeaderHeader.aa = 0;       // authoritative answer
    this->dnsHeaderHeader.tc = 0;       // truncation
    this->dnsHeaderHeader.rd = 1;       // recursion desired
    this->dnsHeaderHeader.ra = 0;       // recursion available
    this->dnsHeaderHeader.z = 0;        //
    this->dnsHeaderHeader.ad = 0;       //
    this->dnsHeaderHeader.cd = 0;       //
    this->dnsHeaderHeader.rcode = 0;    //

    this->dnsHeaderHeader.q_count = htons(1);   // we have only 1 QUESTION_STRUCT (QUESTION_STRUCT count)
    this->dnsHeaderHeader.ans_count = 0;        // answer record count
    this->dnsHeaderHeader.auth_count = 0;       // authoritative record count
    this->dnsHeaderHeader.add_count = 0;        // additional record count

}

DnsHeader::~DnsHeader() {
    cout << "DnsHeader was destroyed..." << endl;
}
