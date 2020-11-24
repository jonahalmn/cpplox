#include "./parser.h"

std::vector<Statement*> Parser::parse() {
    std::vector<Statement *> list;

    while (!is_at_end())
    {
        list.push_back(statement());
    }

    return list;
}

Statement *Parser::statement() {
    std::vector<TokenType> types{TokenType::PRINT};
    if(match(types)) return printStatement();

    return expressionStatement();
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

Expression* Parser::expression() {
    Expression* expr = ternary();

    std::vector<TokenType> types{TokenType::COMMA};

    while (match(types))
    {
        Token token_operator{previous()};
        Expression* right = ternary();
        Binary *binary = new Binary{expr->clone(), token_operator, right->clone()};
        expr = binary;
    }

    return expr;
}

Expression* Parser::ternary() {
    Expression* expr = equality();

    std::vector<TokenType> types{TokenType::QUESTION_MARK};

    while (match(types))
    {
        Expression* consequence = equality();
        consume(TokenType::COLON, "Expect ':'");
        Expression* alternative = equality();
        Ternary *ternary = new Ternary{expr->clone(), consequence->clone(), alternative->clone()};
        expr = ternary;
    }

    return expr;
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

    return primary();
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
