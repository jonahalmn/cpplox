#include <iostream>
#include <map>
#include <any>
#include "../error/runtimeError.h"
#include "../lexer/token.h"

#ifndef ENV_H
#define ENV_H

class Environment {
    std::map<std::string, std::any> m_list{};
    Environment* m_enclosing = nullptr;

    public:
        Environment() {};
        Environment(Environment* enclosing) : m_enclosing{enclosing} {};

        void define(Token token, std::any object);
        std::any get(Token token);
        std::any get_at(int distance, std::string name);
        std::any assign(Token token, std::any object);
        std::any assign_at(int depth, Token token, std::any object);
        Environment *ancestor(int depth);
        void find();
};

#endif