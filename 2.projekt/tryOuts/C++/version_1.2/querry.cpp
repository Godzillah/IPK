/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 20.3.2018 - 14:54
 *  NAME   : Querry.cpp
 */


#include "querry.h"

Querry::Querry() {
    cout << "Starting Querry..." << endl;
    // TODO: qtype : bude z ParseArgs  qclass : bude implicitne 1! (IN = internet)
    // TODO: qname : null ?
    QUESTION_STRUCT.qtype = 2;     // toto bude argument z args
    QUESTION_STRUCT.qclass = 1;    // IN =  1
    query.qname = NULL;     // inicializacia na null
}

u_char* Querry::getQName() {
    return this->query.qname;
}

u_short Querry::getQclass() {
    return this->QUESTION_STRUCT.qclass;
}

u_short Querry::getQtype() {
    return this->QUESTION_STRUCT.qtype;
}

void Querry::setQName(u_char* parName) {
    this->query.qname = parName;
}

void Querry::setQclass(u_short parClass) {
    this->QUESTION_STRUCT.qclass = parClass;
}

void Querry::setQtype(u_short parType) {
    this->QUESTION_STRUCT.qtype = parType;
}

Querry::~Querry() {
    cout << "Ending Querry..." << endl;
}

