#include "./types.h"
#include<iostream>
#include "../parser/function.h"
#include "./loxCallable.h"
#include "./returnExeption.h"
#include <vector>
#include <any>
#include <map>

#ifndef LOXCLASS_H
#define LOXCLASS_H

class LoxClass : public LoxCallable {
    public:

        Token m_name;
        std::map<std::string, LoxCallable *> m_methods;
        LoxClass(Token name, std::map<std::string, LoxCallable *> methods) : m_name{name}, m_methods{methods} {}

        virtual std::any call(Evaluator *evaluator, std::vector<std::any> args);
        virtual unsigned int arity();
        LoxCallable *findMethod(std::string);
};

#endif