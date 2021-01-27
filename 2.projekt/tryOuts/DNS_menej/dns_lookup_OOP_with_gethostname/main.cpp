/* 
 * FILE  : main.cpp
 * AUTHOR: Maros Orsak
 * LOGIN : xorsak02
 * DATE  : 30.3.2018
 */

#include <iostream>

#include "gethostbyname.h"
using namespace std;

/*
 *
 */

int main(int argc, char** argv) {
    
    // constructor called
	Gethostbyname *gethostbyname = new Gethostbyname();
	gethostbyname->run();


	// destructor called explicit
	delete(gethostbyname);

    return 0;
}