
#include <string>
#include "parseArgs.h"
#include "err.h"

// -- testovanie headrov potom vymaz
#include "dnsHeader.h"
#include "querry.h"
#include "rData.h"
#include "responseRecord.h"
#include "dnsClient.h"
// -------------------------------------

using namespace std;

int main(int argc , char** argv){

  cout << "*******MAIN STARTING*******" << endl;

    try {
        ParseArgs *args = new ParseArgs(argc, argv);          // spracovanie argumentov

        cout << "Toto je domenove meno -> " << args->domainName << endl;

        // - TESTOVANIE ... vypisu hostname     |DNSCLIENT|
        DnsClient *dnsHeaderClient;
        dnsHeaderClient->getHostByName((unsigned char*)argv[2],1,argc,argv);
        delete(dnsHeaderClient);
         //-- KONIEC TESTOVANIA







        // - TESTOVANIE... Querry triedy        |QUERRY|
//        Querry querry;
//        cout << querry.getQclass() << endl;
        // - KONIEC TESTOVANIE







        // - TESTOVANIE...                      |RDATA|
//        RData rData;
//        rData.set_Class(2);
//        rData.setTtl(12);
//        cout << "Toto je v ttl:" << rData.getTtl() << endl;
//        cout << "Toto je v class:" << rData.get_Class() << endl;
        // -- KONIEC TESTOVANIA








        // TESTOVANIE...                        |RES_RECORD|
//        ResponseRecord responseRecord;
//        RData rData;

//        responseRecord.setName((u_char*)"Ahoj sa mas");
//        responseRecord.setRData((u_char*)"Cegos...");

//        cout << "Toto je meno " << responseRecord.getName() << endl;
//        cout << "Toto su rData" << responseRecord.getRData() << endl;
//
//                                              |RES_RECORD -> R_DATA data|

//        responseRecord.setRdataType(10);
//        responseRecord.setRdataDatalen(20);
//        responseRecord.setRdataTtl(155);
//        responseRecord.setRdata_Class(2);

//        cout << "Toto su RES_RECORD->R_DATA->type:" << responseRecord.getRDataType() << endl;
//        cout << "Toto su RES_RECORD->R_DATA->dataLen" << responseRecord.getRdataDataLen() << endl;
//        cout << "Toto su RES_RECORD->R_DATA->ttl" << responseRecord.getRdataTtl() << endl;
//        cout << "Toto su RES_RECORD->R_DATA->_class" << responseRecord.getRdataTtl() << endl;


        // delete is a language construct that perform dynamic memory allocation and object destruction
        delete(args);
    }catch (Err &err){
        err.printExit();
    }



    cout << "*******MAIN ENDING*******" << endl;
    return 0;

}

