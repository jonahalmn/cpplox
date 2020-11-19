#include <iostream>
#include <vector>
#include "../lexer/token.h"
#include "./expression.h"
#include "./binary.h"
#include <any>

class Parser {
    private:
        int m_current = 0;
        std::vector<Token> m_tokens;

        Expression* expression();
        Expression* equality();
        Expression* comparison();

        Token advance();
        Token previous();

        bool match(const TokenType[]);
        bool is_at_end();

    public:
        Parser(std::vector<Token> tokens) : m_tokens{tokens} {}


};