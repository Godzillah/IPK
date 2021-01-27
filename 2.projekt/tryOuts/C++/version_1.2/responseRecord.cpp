/*	AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 5.4.2018 - 16:57
 *  NAME   : responseRecord.cpp
 */


#include "responseRecord.h"


ResponseRecord::ResponseRecord() {
    cout << "Starting ResponseRecord" << endl;
    // TESTING |RDATA -> ResponseRecord|
//    struct R_DATA resource = { 1, 1, 3,4};      // inicializacia dat
//    this->res_record.resource = resource;       // priradenie dat do struktury
}

u_char* ResponseRecord::getName() {
    return res_record.name;
}

u_char* ResponseRecord::getRData() {
    return res_record.rData;
}

u_short ResponseRecord::getRDataType() {
    return res_record.resource.type;
}

u_short ResponseRecord::getRData_Class() {
    return res_record.resource._class;
}

int32_t ResponseRecord::getRdataTtl() {
    return res_record.resource.ttl;
}

u_short ResponseRecord::getRdataDataLen() {

    return res_record.resource.dataLen;
}

void ResponseRecord::setName(u_char* parName) {
    this->res_record.name = parName;
}

void ResponseRecord::setRData(u_char* parRData) {
    this->res_record.rData = parRData;
}

void ResponseRecord::setRdataType(u_short parRDataType) {
    this->res_record.resource.type = parRDataType;
}

void ResponseRecord::setRdataDatalen(u_short parRDataLen) {
    this->res_record.resource.dataLen = parRDataLen;
}

void ResponseRecord::setRdataTtl(int32_t parRDataTtl) {
    this->res_record.resource.ttl = parRDataTtl;
}

void ResponseRecord::setRdata_Class(u_short parRData_Class) {
    this->res_record.resource._class = parRData_Class;
}

ResponseRecord::~ResponseRecord() {
    cout << "Ending ResponseRecord" << endl;
}
