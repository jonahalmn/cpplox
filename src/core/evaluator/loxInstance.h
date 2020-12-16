#include "./types.h"
#include<iostream>
#include "../parser/function.h"
#include "./loxCallable.h"
#include "./returnExeption.h"
#include "../error/runtimeError.h"
#include <vector>
#include <any>
#include <map>

#ifndef LOXINST_H
#define LOXINST_H

class LoxInstance {
    public:
        LoxClass *m_klass;
        std::map<Token, std::any> m_fields{};

        LoxInstance(LoxClass *klass) : m_klass{klass} {};
        LoxInstance() {};

        virtual std::any get(Token name);
        virtual std::any set(Token name, std::any value);
        LoxFunction *bind(LoxInstance *);
};

#endif