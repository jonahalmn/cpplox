#include <iostream>
#include "../lexer/token.h"

#ifndef RT_ERROR_H
#define RT_ERROR_H

class RuntimeError {
    public:
        Token m_token;
        std::string m_message;
        RuntimeError(Token token, std::string message) : m_token{token}, m_message{message} {}
};

#endif