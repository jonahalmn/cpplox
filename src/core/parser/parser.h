#include <iostream>
#include <vector>
#include "../lexer/token.h"
#include "./assign.h"
#include "./expression.h"
#include "./statement.h"
#include "./binary.h"
#include "./unary.h"
#include "./literal.h"
#include "./grouping.h"
#include "./ternary.h"
#include "./print.h"
#include "./var.h"
#include "./variable.h"
#include "./stmtexpression.h"
#include "../error/parseError.h"
#include "../errorReporter.h"
#include <any>
#include <cstdlib>

class Parser {
    private:
        unsigned int m_current = 0;
        std::vector<Token> m_tokens;
        ErrorReporter *m_error_reporter = ErrorReporter::getInstance();

        Expression* expression();
        Expression* assigment();
        Expression* ternary();
        Expression* equality();
        Expression* comparison();
        Expression* term();
        Expression* factor();
        Expression* unary();
        Expression* primary();

        Statement* statement();
        Statement* printStatement();
        Statement* expressionStatement();

        Statement* declaration();
        Statement* varDeclaration();

        Token advance();
        Token previous();
        Token peek();
        Token consume(TokenType, std::string);

        bool check(const TokenType);
        bool match(std::vector<TokenType>);
        bool is_at_end();
        void sync();

        ParseError error(Token, std::string);

    public:
        Parser(std::vector<Token> tokens) : m_tokens{tokens} {}

        // Expression* parse();
        std::vector<Statement*> parse();

};