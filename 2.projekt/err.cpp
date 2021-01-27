/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 21.3.2018 - 12:48
 *  NAME   : err.h
 */

#include "err.h"

// dopracovat hodnoty vracania 
//			0 -> v pripade uspechu
//			1 -> v pripade neuspechu (neexistujuci zaznam, vyprsenie timeoutu) 
//			2 -> pre chybne zadane parametre pri spusteni 

Err::Err(string message) {
    this->returnValue = 2;
    this->message = message;
}


Err::Err(string message, int returnValue) {
    this->returnValue = returnValue;
    this->message = message;
}

void Err::print(){
    cerr << "Error--> " << this->message << " Code Error--> " << this->returnValue << " \n" ; 
}

void Err::printExit(){
    cerr << this->message <<  endl; 
    exit(this->returnValue);
}

Err::~Err() {
}