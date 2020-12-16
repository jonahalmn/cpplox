#include "./types.h"
#include<iostream>
#include "../parser/function.h"
#include "./loxCallable.h"
#include "./loxInstance.h"
#include "./returnExeption.h"
#include "../error/nonStaticError.h"
#include <vector>
#include <any>
#include <map>

#ifndef LOXCLASS_H
#define LOXCLASS_H

class LoxClass : public LoxCallable, public LoxInstance {
    public:

        Token m_name;
        std::map<std::string, LoxCallable *> m_methods;
        std::map<std::string, LoxCallable *> m_statics;
        std::map<std::string, LoxCallable *> m_getters;

        LoxClass(
            Token name,
            std::map<std::string, LoxCallable *> methods,
            std::map<std::string, LoxCallable *> statics,
            std::map<std::string, LoxCallable *> getters
        ) : m_name{name}, m_methods{methods}, m_statics{statics}, m_getters{getters} {}

        virtual std::any call(Evaluator *evaluator, std::vector<std::any> args);
        virtual unsigned int arity();
        virtual std::any get(Token name);
        // virtual std::any set(Token name, std::any value);
        LoxCallable *findMethod(std::string);
        LoxCallable *findStatic(std::string);
        LoxCallable *findGetter(std::string);
};

#endif