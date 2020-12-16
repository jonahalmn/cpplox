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
        bool m_is_initializer = false;

    public:
        LoxFunction(Function *declaration, Environment *closure, bool is_initializer) : m_declaration{declaration}, m_closure{closure}, m_is_initializer{is_initializer} {};
        virtual unsigned int arity();
        virtual std::any call(Evaluator *evaluator, std::vector<std::any> arguments);
        virtual LoxFunction *bind(LoxInstance *);
        virtual ~LoxFunction(){};
};

#endif