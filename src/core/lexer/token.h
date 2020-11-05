#include <iostream>
#include <string>
#include "../../inc/token_types.h"

#ifndef TOKEN_H
#define TOKEN_H

class Token {
    private:

    public:

        TokenType m_token_type;
        std::string m_lexeme;
        unsigned int m_line;

        Token(){};

        Token(TokenType token_type, const char* lexeme, unsigned int line) :
            m_token_type{token_type}, m_lexeme{lexeme}, m_line{line} {}

        friend std::ostream& operator<<(std::ostream& os, const Token& token);

};

#endif