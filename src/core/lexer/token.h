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

        bool operator==(const Token& token) {
            if(m_lexeme == token.m_lexeme && m_token_type == token.m_token_type) {
                return true;
            }

            return false;
        }

        inline bool operator< (const Token& token) const { 
            return m_lexeme > token.m_lexeme;
        }

};

#endif