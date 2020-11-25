#include <iostream>
#include <map>
#include <any>
#include "../error/runtimeError.h"
#include "../lexer/token.h"

class Environment {
    std::map<std::string, std::any> m_list{};
    public:
        void define(Token token, std::any object);
        std::any get(Token token);
        std::any assign(Token token, std::any object);
};