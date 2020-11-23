#include <iostream>
#include "../lexer/token.h"

class RuntimeError {
    private:
        Token m_token;
        std::string m_message;
    public:
        RuntimeError(Token token, std::string message) : m_token{token}, m_message{message} {}
};