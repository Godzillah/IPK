/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   arguments.h
 * Author: matus
 *
 * Created on Utorok, 2017, apríla 18, 18:04
 */

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <string>
#include <string.h>
#include "Error.h"

class Arguments {
public:
    int first_ttl;
    int max_ttl;
    int port;
    std::string ip_address;
    Arguments(int argc, char **argv);
    virtual ~Arguments();
private:

};

#endif /* ARGUMENTS_H */

