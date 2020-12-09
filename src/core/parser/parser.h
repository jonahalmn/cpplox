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
#include "./block.h"
#include "./stmtexpression.h"
#include "./ifstmt.h"
#include "./whilestmt.h"
#include "./logical.h"
#include "./breakstmt.h"
#include "./call.h"
#include "../error/parseError.h"
#include "../errorReporter.h"
#include "./function.h"
#include "./returnstmt.h"
#include <any>
#include <cstdlib>

class Parser {
    private:
        unsigned int m_current = 0;
        std::vector<Token> m_tokens;
        ErrorReporter *m_error_reporter = ErrorReporter::getInstance();
        int m_loop_level = 0;

        Expression* expression();
        Expression* assigment();
        Expression* ternary();
        Expression* equality();
        Expression* comparison();
        Expression* term();
        Expression* factor();
        Expression* unary();
        Expression* call();
        Expression* primary();

        Expression* finishCall(Expression *);

        Expression* orExpr();
        Expression* andExpr();

        Statement* block();
        Statement* function(std::string);

        Statement* statement();
        Statement* printStatement();
        Statement* expressionStatement();

        Statement* ifStatement();
        Statement* whileStatement();
        Statement* forStatement();
        Statement* breakStatement();
        Statement* returnStatement();

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
        bool is_breakable();

        ParseError error(Token, std::string);

    public:
        Parser(std::vector<Token> tokens) : m_tokens{tokens} {}

        // Expression* parse();
        std::vector<Statement*> parse();

};