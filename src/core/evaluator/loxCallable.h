#include "./types.h"
#include<iostream>
#include<any>
#include<vector>

#ifndef CALLABLE_H
#define CALLABLE_H

class LoxCallable {
    public:
        virtual unsigned int arity() = 0;
        virtual std::any call(Evaluator *evaluator, std::vector<std::any> arguments) = 0;
        virtual ~LoxCallable(){};
};

#endif