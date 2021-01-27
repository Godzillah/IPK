/* 
 * File:   Error.cpp
 * Author: matus
 * 
 * Created on Nedeľa, 2017, marca 12, 13:15
 */

#include "Error.h"


Error::Error(string msg, int code) {
    this->code = code;
    this->msg = msg;
}

Error::Error(string msg) {
    this->code = -1;
    this->msg = msg;
}

void Error::Print(){
    cerr << "Chyba: " << this->msg << "(Kod chyby: " << this->code << " )\n" ; 
}

void Error::PrintAndExit(){
    cerr << this->msg <<  endl; 
    exit(this->code);
}

Error::~Error() {
}

