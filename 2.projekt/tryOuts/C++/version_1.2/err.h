/*
 *  AUTHOR : MAROS ORSAK
 *  LOGIN  : XORSAK02
 *  TIME   : 21.3.2018 - 12:44
 *  NAME   : Err.h
 */

#ifndef ERR_H
#define ERR_H

#include <string>
#include <iostream>

using namespace std;

class Err {
public:
    string message;
    int returnValue;
    
    Err(string message, int returnValue);
    Err(string message);
    void print();
    void printExit();
    virtual ~Err();

private:
protected:
    
};

#endif /* ERR_H  */

