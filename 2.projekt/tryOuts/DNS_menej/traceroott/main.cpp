/* 
 * File:   main.cpp
 * Author: matus
 *
 * Created on Utorok, 2017, apríla 18, 17:58
 */

#include <cstdlib>
#include "Error.h"
#include "Arguments.h"
#include "Sender.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
   try{
        
        Arguments *args = new Arguments(argc, argv);    // spracovanie argumentov
        Sender *send = new Sender( args->ip_address.c_str() , args->port);  // vytvorenie socketu, spracovanie IP adresy/hostname
        for( int i = args->first_ttl; i <= args->max_ttl; i++)  // posielanie paketov od first_ttl po max_ttl
            if (send->send(i))  // vrati true ak sme nasli ciel
                break;

    }
    catch(Error &e)
    {
        e.PrintAndExit();
    }
    return 0;
}

