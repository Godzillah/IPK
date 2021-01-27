/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 20.3.2018 - 23:35
 *  NAME   : ParseArgs.cpp
 */


#include "parseArgs.h"
#include "err.h"
#include <cstdlib>
#include <stdlib.h>



// konstruktor
ParseArgs::ParseArgs(int argc, char **argv) {

    cout << "Starting parsing..." << endl;

    if(argc == 1){                                                                  // akonahle bude pocet argumentov 1 error a suggest na napovedu
        throw Err("Zle zadane parametre pre zobrazenie napovedy : ./ipk-lookup -h");
    }
    else if(argc > 9){                                                              // ak bude pocet argumentov vacsi ako 9
        throw Err("Prilis vela zadanych parametrov");
    }
    else if (argc == 2 ){                          // ak bude pocet argumentov 2 a zaroven 1 argument bude rovny -h --> tlac napovedu
        if(strcmp(argv[1] , "-h") == 0){
            help();								   // napoveda

        }else{
            throw Err("Zle zadany parameter");
        }

    }

    else if(argc == 3){
        throw Err("Nezadana prekladacie domena");
    }

    else if(argc < 10 ){                                                            // ak bude pocet argumentov mensi ako 10 a zaroven nebudu v parnom pocte pokracuj
      
      	// -s musi byt na zaciatku spolu s adresou 
       if((strcmp(argv[1] , "-s") == 0)){
            if(argc == 4){
                cout << "Parameter |-s|:DNS server (IPv4 adresa), na který se budou odesílat dotazy" << endl;
                this->ip4Address = argv[2];
                this->domainName = argv[3]; // ak bude napr : ./ipk-lookup -s server name -> tak priradime na 4 poziciu name...
                this->type = "A";           // default : type    = A
                this->timeOut = 5;          // default : timeout = 5s

                cout << "Prekladame domenu :" << this->domainName << endl;
                cout << "Default timeout :" << this->timeOut << endl;
                cout << "Default type :" << this->type << endl;
            }
            else if(argc > 4){
                cout << "Parameter(argv > 4) |-s|:DNS server (IPv4 adresa), na který se budou odesílat dotazy" << endl;
                this->ip4Address = argv[2];
                cout << "ipv4 adresa :" << this->ip4Address << endl;
            }


       }
       else{
       		throw Err("Nedany nazov serveru");
       }

      for(int i = 3 ; i + 1 < argc ; i = i + 2 )    {                                       // bude prechadzat kazdy neparny argument )-T -t -i...
           if(argc == 4){           // pre : ./ipk-lookup -s server name
               break;
           }

           else if((strcmp(argv[i] , "-T") == 0)){

               if((argc % 2) || ((argc == 6 && strcmp(argv[5] , "-i") == 0 ) == 0) || ((argc == 9 && strcmp(argv[8] , "-i") == 0 ) == 0)) {    // kvoli ./ipk-lookup -s server -t AAAA -i name
                   cout << "Parameter |-T|:volitelný parametr, timeout (v sekundách) pro dotaz , pre zvolenu hodnotu" << endl;
                   if(((argc == 6 && strcmp(argv[5] , "-i") == 0 ) == 0) || ((argc == 9 && strcmp(argv[8] , "-i") == 0 ) == 0)){
                       this->domainName = argv[i+3];
                   }
                   else{
                       this->domainName = argv[i+2];

                   }
                   this->timeOut = atoi(argv[i+1]);
                   cout << "toto je v -T domenove meno:" << this->domainName << endl;
                   cout << "toto je zvolena hodnota " << this->timeOut << endl;
               }
               else{
                   throw Err("Nezadana hodnota -T prepinaca");
               }
           }
           else if((strcmp(argv[i] , "-t") == 0)){
                   if((argc % 2) || ((argc == 6 && strcmp(argv[5] , "-i") == 0 ) == 0) || ((argc == 9 && strcmp(argv[8] , "-i") == 0 ) == 0)) {    // kvoli ./ipk-lookup -s server -t AAAA -i name
                      cout << "Parameter |-t|:volitelný parametr, typ dotazovaného záznamu: A (výchozí), AAAA, NS, PTR, CNAME." << endl;
                      if(((argc == 6 && strcmp(argv[5] , "-i") == 0 ) == 0) || ((argc == 9 && strcmp(argv[8] , "-i") == 0 ) == 0)){
                          this->domainName = argv[i+3];
                      }
                      else{
                          this->domainName = argv[i+2];

                      }
                      this->type = argv[i+1];
                      cout << "toto je v -t domenove meno:" << this->domainName << endl;
                      cout << "toto je zvolena hodnota: " << this->type << endl;
                   }
                   else{
                       throw Err("Nezadana hodnota -t prepinaca");
                   }

          }
          else if((strcmp(argv[i] , "-i") == 0)){
               cout << "i (iterative) - volitelný parametr, vynucení iterativního způsobu rezoluce, viz dále." << endl;
          }

          else if(i == 7){
              break;
          }

          else{
              throw Err("Chybne parametre");
          }
      }

    }
    else{
        throw Err("Fail parametetre");
    }
}


void ParseArgs::help(){

	cout << "**************************************************************************\n" 
	 		"* Usage: ./ipk-lookup [options]						 *\n"
			"*	Options:							 *\n" 
	 		"*		./ipk-lookup [-h]					 *\n"
	 		"*		./ipk-lookup -s server name 				 *\n" 
	 		"*		./ipk-lookup -s server [-T timeout] [-t type] [-i] name  *\n" 
	 		"**************************************************************************\n"; 
}

// destruktor
ParseArgs::~ParseArgs() {
    cout << "Ending parsing..." << endl;
}