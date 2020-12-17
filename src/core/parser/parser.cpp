#include "./parser.h"

std::vector<Statement*> Parser::parse() {
    std::vector<Statement *> list;

    while (!is_at_end())
    {
        list.push_back(declaration());
    }

    return list;
}

Statement *Parser::declaration() {
    try {
        std::vector<TokenType> types{TokenType::VAR};
        if(match(types)) return varDeclaration();
        if(match(std::vector<TokenType>{TokenType::FUN})) return function("function");
        if(match(std::vector<TokenType>{TokenType::CLASS})) return classDeclaration();
        return statement();
    } catch (ParseError) {
        sync();
    }

    std::cout << "kill all" << std::endl;
    exit(70);
}

Statement *Parser::classDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "need class identifier");

    Variable *superclass = nullptr;
    if(match(std::vector<TokenType>{TokenType::LESS})) {
        consume(TokenType::IDENTIFIER, "need superclass identifier");
        superclass = new Variable{previous()};
    }

    consume(TokenType::LEFT_BRACE, "need { after class declaration");

    std::vector<Function *> methods{};
    std::vector<Function *> statics{};
    std::vector<Function *> getters{};

    while (!check(TokenType::RIGHT_BRACE) && !is_at_end())
    {
        if(match(std::vector<TokenType>{TokenType::CLASS})) {
            statics.push_back(static_cast<Function *>(function("static method")));
        } else {
            try {
                methods.push_back(static_cast<Function *>(function("method")));
            } catch(GetterException e) {
                getters.push_back(static_cast<Function *>(function("getter")));
            }
        }
    }

    consume(TokenType::RIGHT_BRACE, "expect } at end of class");

    return new ClassDecl{name, superclass, methods, statics, getters};
}

Statement *Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Identifier needed");

    Expression *initializer = nullptr;
    std::vector<TokenType> types{TokenType::EQUAL};
    if(match(types)) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ;");

    Var *var = new Var{name, initializer};
    return var;
}

Statement *Parser::function(std::string kind) {
    Token name;
    if(kind != "getter") {
        name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name");
    } else {
        name = previous();
    }

    if(kind == "method" && check(TokenType::LEFT_BRACE)) throw GetterException();
    std::vector<Token> parameters{};
    if(kind != "getter") {
        consume(TokenType::LEFT_PAREN, "Expect ( after " + kind + " name");

        if(!check(TokenType::RIGHT_PAREN)) {
            do {
                if(parameters.size() >= 255) {
                    error(peek(), "Can't have more than 255 params");
                }

                parameters.push_back(consume(TokenType::IDENTIFIER, "Parameter needed"));
            } while(match(std::vector<TokenType>{TokenType::COMMA}));
        }

        consume(TokenType::RIGHT_PAREN, "Expect ) after params list");
    }

    consume(TokenType::LEFT_BRACE, "Expect { before " + kind + " body");
    Block *body = static_cast<Block *>(block());
    return new Function{name, parameters, body};
}

Statement *Parser::breakStatement() {
    Token token = previous();
    consume(TokenType::SEMICOLON, "need ; after break");

    return new BreakStmt{token};
}

Statement *Parser::statement() {
    if(match(std::vector<TokenType> {TokenType::PRINT}))
        return printStatement();

    if(match(std::vector<TokenType>{TokenType::LEFT_BRACE}))
        return block();

    if(match(std::vector<TokenType>{TokenType::IF})) {
        return ifStatement();
    }

    if(match(std::vector<TokenType>{TokenType::BREAK}) && is_breakable())
        return breakStatement();

    if(match(std::vector<TokenType>{TokenType::BREAK}))
        throw error(peek(), "Can't break outside a loop.");

    if(match(std::vector<TokenType>{TokenType::FOR})) {
        return forStatement();
    }

    if(match(std::vector<TokenType>{TokenType::RETURN}))
        return returnStatement();

    return expressionStatement();
}

Statement *Parser::returnStatement() {
    Token keyword = previous();
    Expression* to_return = nullptr;
    if(!check(TokenType::SEMICOLON)) {
        to_return = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ; after return value");

    return new ReturnStmt{keyword, to_return};
}

Statement *Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Condition must start with (");
    Expression* condition = expression();
    consume(TokenType::RIGHT_PAREN, "Condition must end with )");
    m_loop_level++;
    Statement *body = statement();
    m_loop_level--;

    return new WhileStmt{condition, body};
}

Statement *Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Condition must start with (");
    Expression* condition = expression();
    consume(TokenType::RIGHT_PAREN, "Condition must end with )");
    Statement* thenBranch = statement();
    Statement* elseBranch = nullptr;

    if(match(std::vector<TokenType>{TokenType::ELSE})) {
        elseBranch = statement();
    }

    return new IfStmt(condition, thenBranch, elseBranch);
}

Statement *Parser::forStatement() {
    consume(TokenType::LEFT_PAREN, "Condition must start with (");

    Statement *initializer;

    if(match(std::vector<TokenType>{TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else if(match(std::vector<TokenType>{TokenType::VAR})) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }

    Expression *condition = nullptr;

    if(!check(TokenType::SEMICOLON)) {
        condition = expression();
    }

    consume(TokenType::SEMICOLON, "need semicolon after condition");


    Expression *increment = nullptr;

    if(!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }

    consume(TokenType::RIGHT_PAREN, "expected closing parenthesis");

    m_loop_level++;

    Statement *body = statement();

    if(increment != nullptr) {
        body = new Block{
            new std::vector<Statement *>{
                body->clone(),
                new StmtExpression( increment )
            }
        };
    }

    if(condition == nullptr) condition = new Literal(false);
    body = new WhileStmt(condition, body);

    if(initializer != nullptr) {
        body = new Block{
            new std::vector<Statement *>{
                initializer,
                body->clone()
            }
        };
    }

    m_loop_level--;

    return body;
}

Statement *Parser::block() {
    std::vector<Statement *> *stmts = new std::vector<Statement *>;

    while(!check(TokenType::RIGHT_BRACE) && !is_at_end()) {
        stmts->push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block");

    Block *block = new Block{ stmts };
    return block;
}

Statement *Parser::printStatement() {
    Expression *value = expression();
    consume(TokenType::SEMICOLON, "Expect ; after statement");
    Print *stmt = new Print{value};
    return stmt;
}

Statement * Parser::expressionStatement() {
    Expression *value = expression();
    consume(TokenType::SEMICOLON, "Expect ; after expression");
    Statement *stmt = new StmtExpression{value};
    return stmt;
    // try {
    // } catch(ParseError e) {
    //     return NULL;
    // }
}

Expression* Parser::assigment() {
    Expression* expr = ternary();

    std::vector<TokenType> types{TokenType::EQUAL};

    if(match(types)) {
        Token equals{previous()};
        Expression* value = assigment();

        if(Variable *variable = dynamic_cast<Variable *>(expr)) {
            Token name = variable->m_name;
            Assign *assign = new Assign{name, value};
            return assign;
        } else if (Get *get = dynamic_cast<Get *>(expr)){
            return new Set{get->m_object, get->m_name, value};
        }

        error(equals, "Invalid assigment target.");
    }

    return expr;
}

Expression* Parser::expression() {
    Expression* expr = assigment();

    std::vector<TokenType> types{TokenType::COMMA};

    while (match(types))
    {
        Token token_operator{previous()};
        Expression* right = assigment();
        Binary *binary = new Binary{expr->clone(), token_operator, right->clone()};
        expr = binary;
    }

    return expr;
}

Expression* Parser::ternary() {
    Expression* expr = orExpr();

    std::vector<TokenType> types{TokenType::QUESTION_MARK};

    while (match(types))
    {
        Expression* consequence = orExpr();
        consume(TokenType::COLON, "Expect ':'");
        Expression* alternative = orExpr();
        Ternary *ternary = new Ternary{expr->clone(), consequence->clone(), alternative->clone()};
        expr = ternary;
    }

    return expr;
}

Expression* Parser::orExpr() {
    Expression *left = andExpr();

    while (match(std::vector<TokenType> {TokenType::OR}))
    {
        Token operator_or = previous();
        Expression *right = andExpr();
        return new Logical(left, operator_or, right);
    }

    return left;

}

Expression* Parser::andExpr() {
    Expression *left = equality();

    while (match(std::vector<TokenType> {TokenType::AND}))
    {
        Token operator_and = previous();
        Expression *right = equality();
        return new Logical(left, operator_and, right);
    }

    return left;

}

Expression* Parser::equality() {
    Expression* expr = comparison();


    std::vector<TokenType> types{TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL};

    while (match(types))
    {
        Token token_operator{previous()};
        Expression* right = comparison();
        Binary *binary = new Binary{expr->clone(), token_operator, right->clone()};
        expr = binary;
    }

    return expr;
}

Expression* Parser::comparison() {
    Expression* expr = term();

    std::vector<TokenType> types {TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL};

    while(match(types)) {
        Token token_operator{previous()};
        Expression* right = term();
        Binary *binary = new Binary{expr->clone(), token_operator, right->clone()};
        expr = binary;
    }

    return expr;
}

Expression* Parser::term() {
    Expression* expr = factor();

    std::vector<TokenType> types {TokenType::MINUS, TokenType::PLUS};

    while(match(types))  {
        Token token_operator{previous()};
        Expression* right = factor();
        Binary *binary = new Binary{expr->clone(), token_operator, right->clone()};
        expr = binary;
    }

    return expr;
}

Expression* Parser::factor() {
    Expression* expr = unary();
    std::vector<TokenType> types{TokenType::SLASH, TokenType::STAR};

    while(match(types))  {
        Token token_operator{previous()};
        Expression* right = unary();
        Binary *binary = new Binary{expr->clone(), token_operator, right->clone()};
        expr = binary;
    }

    return expr;
}

Expression* Parser::unary() {

    std::vector<TokenType> types {TokenType::BANG, TokenType::MINUS};

    if(match(types)) {
        Token token_operator{previous()};
        Expression* right = unary();
        Unary *unary = new Unary{token_operator, right->clone()};
        return unary;
    }

    return call();
}

Expression* Parser::call() {
    Expression *expr = primary();

    while (true)
    {
        if(match(std::vector<TokenType>{TokenType::LEFT_PAREN})) {
            expr = finishCall(expr);
        } else if(match(std::vector<TokenType>{TokenType::DOT})) {
            Token name = consume(TokenType::IDENTIFIER, "expect property name after '.'");
            expr = new Get{expr, name};
        } else {
            break;
        }
    }

    return expr;
}

Expression* Parser::finishCall(Expression *expr) {
    std::vector<Expression *> arguments{};

    if(!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(assigment());
        } while(match(std::vector<TokenType>{TokenType::COMMA}));
    }

    if(arguments.size() > 255) {
        error(peek(), "can't pass more than 255 arguments to functions.");
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Need ) after argument list");

    return new Call{expr, paren, arguments};
}

Expression* Parser::primary() {

    std::vector<TokenType> F{TokenType::FALSE};
    if(match(F)) {
        Literal *literal = new Literal{false};
        return literal;
    }

    std::vector<TokenType> T{TokenType::TRUE};
    if(match(T)) {
        Literal *literal = new Literal{true};
        return literal;
    }

    std::vector<TokenType> N{TokenType::NUMBER};
    if(match(N)) {
        Literal *literal = new Literal{std::atof(previous().m_lexeme.c_str())};
        return literal;
    }

    std::vector<TokenType> S{TokenType::STRING};
    if(match(S)) {
        Literal *literal = new Literal{previous().m_lexeme};
        return literal;
    }

    std::vector<TokenType> PAREN{TokenType::LEFT_PAREN};
    if(match(PAREN)) {
        Expression *expr = expression();
        consume(TokenType::RIGHT_PAREN, "')' expected");
        Grouping *grouping = new Grouping{ expr->clone() };
        return grouping;
    }

    std::vector<TokenType> ID{TokenType::IDENTIFIER};
    if(match(ID)) {
        Variable *var = new Variable{previous()};
        return var;
    }

    if(match(std::vector<TokenType>{TokenType::THIS})) {
        return new ThisExpr{previous()};
    }

    if(match(std::vector<TokenType>{TokenType::SUPER})) {
        Token keyword = previous();
        consume(TokenType::DOT, "'.' needed after super keyword");
        Token method = consume(TokenType::IDENTIFIER, "expect class method name");
        return new SuperExpr{keyword, method};
    }

    throw error(peek(), "Expect expression.");


}


bool Parser::match(std::vector<TokenType> types) {
    for(TokenType type : types ) {
        if(peek().m_token_type == type) {
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::is_breakable() {
    return m_loop_level != 0;
}

Token Parser::previous() {
    return m_tokens[m_current - 1];
}

Token Parser::advance() {
    if(!is_at_end()) m_current++;
    return previous();
}

Token Parser::peek() {
    return m_tokens[m_current];
}

bool Parser::is_at_end() {
    return peek().m_token_type == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) {
    if(is_at_end()) return false;
    return peek().m_token_type == type;
}

void Parser::sync() {
    advance();

    while (!is_at_end())
    {
        if(previous().m_token_type == TokenType::SEMICOLON) return;

        switch (peek().m_token_type)
        {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
            break;
        default:
            break;
        }

        advance();
    }
}

Token Parser::consume(TokenType type, std::string message) {
    if(check(type)) return advance();

    throw error(peek(), message);
}

ParseError Parser::error(Token token, std::string message) {
    m_error_reporter->error(token, message.c_str());
    ParseError error_object;
    return error_object;
}
