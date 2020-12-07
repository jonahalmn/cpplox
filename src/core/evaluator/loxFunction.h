#include "./types.h"
#include<iostream>
#include "../parser/function.h"
#include "./loxCallable.h"
#include <vector>
#include <any>

#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H

class LoxFunction : public LoxCallable {
    private:
        Function *m_declaration;

    public:
        LoxFunction(Function *declaration) : m_declaration{declaration} {};
        virtual unsigned int arity();
        virtual std::any call(Evaluator *evaluator, std::vector<std::any> arguments);
        virtual ~LoxFunction(){};
};

#endif