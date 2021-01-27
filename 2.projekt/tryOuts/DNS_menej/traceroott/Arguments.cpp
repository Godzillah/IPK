/* 
 * File:   arguments.cpp
 * Author: matus
 * 
 * Created on Utorok, 2017, apríla 18, 18:04
 */



#include "Arguments.h"

Arguments::Arguments(int argc, char **argv) {
    if (argc == 1)
        throw Error("Na spustenie programu je potrebna aspon IP adresa.");
    
    if((argc - 1) % 2 == 0) //ak je pocet zadanych argumentov parny tak chyba
        throw Error("Zle zadane argumenty.");
    
    if (argc > 8)
        throw Error("Prilis vela argumentov.");
    
    this->first_ttl = 1;
    this->max_ttl = 30;
    this->port = 33434;
    this->ip_address = std::string(argv[argc - 1]);
    for(int  i = 1; i < argc; i +=2){
        
        if( strcmp(argv[i], "-m") == 0)
            this->max_ttl = atoi(argv[i+1]);
        
        if( strcmp(argv[i], "-f") == 0)
            this->first_ttl = atoi(argv[i+1]);  
        if( strcmp(argv[i], "-p") == 0)
            this->port = atoi(argv[i+1]);
    }
    
    if(first_ttl > max_ttl)
        throw Error("chybne zadane argumenty.");
}


Arguments::~Arguments() {
}

