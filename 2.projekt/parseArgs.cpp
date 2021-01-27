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

    if(argc == 1){                                                                  // akonahle bude pocet argumentov 1 error a suggest na napovedu
        throw Err("Zle zadane parametre pre zobrazenie napovedy : ./ipk-lookup -h",2);
    }
    else if(argc > 9){                                                              // ak bude pocet argumentov vacsi ako 9
        throw Err("Prilis vela zadanych parametrov",2);
    }
    else if (argc == 2 ){                          // ak bude pocet argumentov 2 a zaroven 1 argument bude rovny -h --> tlac napovedu
        if(strcmp(argv[1] , "-h") == 0){
            help();								   // napoveda
            exit(0);

        }else{
            throw Err("Zle zadany parameter",2);
        }

    }

    else if(argc == 3){
        throw Err("Nezadana prekladacie domena",2);
    }

    else if(argc < 10 ){                                                            // ak bude pocet argumentov mensi ako 10 a zaroven nebudu v parnom pocte pokracuj
      
      	// -s musi byt na zaciatku spolu s adresou 
       if((strcmp(argv[1] , "-s") == 0)){
            if(argc == 4){
                regex ipv4("\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b");
                // controlling if it is valid ipv4

                if (regex_match(argv[2],ipv4)){
                    this->ip4Address = argv[2];
                }
                else{
                    throw Err("Not a valid IPv4 for DNS server",2);
                }
                this->domainName = argv[3]; // ak bude napr : ./ipk-lookup -s server name -> tak priradime na 4 poziciu name...
                this->type = A;           // default : type    = A
                this->timeOut = 5;          // default : timeout = 5s
            }
            else if(argc > 4){
                regex ipv4("\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b");
                // controlling if it is valid ipv4

                if (regex_match(argv[2],ipv4)){
                    this->ip4Address = argv[2];
                }
                else{
                    throw Err("Not a valid IPv4 for DNS server",2);
                }
                this->type = A;           // default : type    = A
                this->timeOut = 5;          // default : timeout = 5s
            }


       }
       else{
       		throw Err("Nedany nazov serveru",2);
       }

      for(int i = 3 ; i + 1 < argc ; i = i + 2 )    {                                       // bude prechadzat kazdy neparny argument )-T -t -i...
           if(argc == 4){           // pre : ./ipk-lookup -s server name
               break;
           }

           else if((strcmp(argv[i] , "-T") == 0)){

               if((argc % 2 == 0 ) || ((argc == 6 && strcmp(argv[5] , "-i") == 0 ) == 0) || ((argc == 9 && strcmp(argv[8] , "-i") == 0 ) == 0)) {    // kvoli ./ipk-lookup -s server -t AAAA -i name

                   if(((argc == 6 && strcmp(argv[5] , "-i") == 0 ) == 0) || ((argc == 9 && strcmp(argv[8] , "-i") == 0 ) == 0)){
                       this->domainName = argv[i+2];
                   }
                   else{
                       this->domainName = argv[i+1];

                   }

                   if(atoi(argv[i+1]) > 0){
                       this->timeOut = atoi(argv[i+1]);
                   }
                   else{
                       throw Err("Zadali ste zaporny timeOut");
                   }
               }
               else{
                   throw Err("Nezadana hodnota -T prepinaca",1);
               }
           }
           else if((strcmp(argv[i] , "-t") == 0)){
                   if((argc % 2 == 0 ) || ((argc == 6 && strcmp(argv[5] , "-i") == 0 ) == 0) || ((argc == 9 && strcmp(argv[8] , "-i") == 0 ) == 0)) {    // kvoli ./ipk-lookup -s server -t AAAA -i name
                      if(((argc == 6 && strcmp(argv[5] , "-i") == 0 ) == 0) || ((argc == 9 && strcmp(argv[8] , "-i") == 0 ) == 0)){
                          this->domainName = argv[i+2];
                      }
                      else{
                          this->domainName = argv[i+1];

                      }
                      if((strcmp(argv[i+1] , "AAAA") == 0)){
                          this->type = AAAA;
                          // regex for matching ipv6
                          regex e("([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]"
                                  "{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]"
                                  "{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|"
                                  "[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4})"
                                  "{0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.)"
                                  "{3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}"
                                  "[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]):");
                          bool match = regex_match(argv[i+2],e);
                          if(match == 0){
                              throw Err("Not a IPv6 adress",2);     // returning 2... because of bad type of args
                          }

                      }
                      else if((strcmp(argv[i+1] , "NS") == 0)){
                           this->type = NS;
                       }
                      else if((strcmp(argv[i+1] , "PTR") == 0)){
                           this->type = PTR;
                          regex ipv6("([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]"
                                  "{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]"
                                  "{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|"
                                  "[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4})"
                                  "{0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.)"
                                  "{3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}"
                                  "[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]):");
                          regex ipv4("\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b");


                          bool match;

                          if((regex_match(argv[i+2],ipv6)) || (regex_match(argv[i+2],ipv4))){
                              match = 1;
                          }
                          else if (match == 0){
                              throw Err("Not a IPv6 or IPv4 adrress ",2);     // returning 2... because of bad type of args
                          }
                       }
                       else if((strcmp(argv[i+1] , "CNAME") == 0)){
                          this->type = CNAME;
                      }
                      else{
                          throw Err("Neznamy RR zaznam",1);
                      }
                   }
                   else{
                       throw Err("Nezadana hodnota -t prepinaca",1);
                   }

          }
          else if((strcmp(argv[i] , "-i") == 0)){
                this->domainName = argv[i+1];
           }

          else if(i == 7){
              break;
          }

          else{
              throw Err("Chybne parametre",2);
          }
      }

    }
    else{
        throw Err("Fail parametetre",2);
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
}