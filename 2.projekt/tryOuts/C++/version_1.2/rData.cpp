/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 5.4.2018 - 15:34
 *  NAME   : rData.cpp
 */

#include "rData.h"


RData::RData() {
    cout << "Starting Rdata..." << endl;
}

// GETTERS

u_short RData::get_Class() {
    return this->r_data._class;
}

u_short RData::getDataLen() {
    return this->r_data.dataLen;
}

int32_t RData::getTtl() {
    return this->r_data.ttl;
}

u_short RData::getType() {
    return this->r_data.type;
}


R_DATA RData::getRData() {
    return this->r_data;
}


// SETTERS

void RData::set_Class(u_short par_Class) {
    this->r_data._class = par_Class;
}

void RData::setDataLen(u_short parDataLen) {
    this->r_data.dataLen = parDataLen;
}

void RData::setTtl(int32_t parTtl) {
    this->r_data.ttl = parTtl;
}

void RData::setType(u_short parType)  {
    this->r_data.type = parType;
}




RData::~RData() {
    cout << "Ending Rdata..." << endl;
}