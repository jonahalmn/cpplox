#include "./parser.h"

Expression * Parser::parse() {
    try {
        return expression();
    } catch(ParseError e) {
        return NULL;
    }
}

Expression* Parser::expression() {
    return equality();
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
    Literal *literal = new Literal{NULL};

    std::vector<TokenType> F{TokenType::FALSE};
    if(match(F)) {
        literal->m_value = false;
        return literal;
    }

    std::vector<TokenType> T{TokenType::TRUE};
    if(match(T)) {
        literal->m_value = true;
        return literal;
    }

    std::vector<TokenType> NT{TokenType::NUMBER, TokenType::STRING};
    if(match(NT)) {
        literal->m_value = previous().m_lexeme;
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
