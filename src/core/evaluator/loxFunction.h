#include "./types.h"
#include<iostream>
#include "../parser/function.h"
#include "./loxCallable.h"
#include "./returnExeption.h"
#include <vector>
#include <any>

#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H

class LoxFunction : public LoxCallable {
    private:
        Function *m_declaration;
        Environment *m_closure;

    public:
        LoxFunction(Function *declaration, Environment *closure) : m_declaration{declaration}, m_closure{closure} {};
        virtual unsigned int arity();
        virtual std::any call(Evaluator *evaluator, std::vector<std::any> arguments);
        virtual ~LoxFunction(){};
};

#endif