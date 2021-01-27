/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Error.h
 * Author: matus
 *
 * Created on Nedeľa, 2017, marca 12, 13:15
 */

#ifndef ERROR_H
#define ERROR_H


#include <string>
#include <iostream>

using namespace std;

class Error {
public:
    int code;
    string msg;
    
    
    Error(string msg, int code);
    Error(string msg);
    void Print();
    void PrintAndExit();
    virtual ~Error();
    

private:
    
};

#endif /* ERROR_H */

