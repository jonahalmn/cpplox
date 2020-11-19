#include "./parser.h"

Expression* Parser::expression() {
    return equality();
}

Expression* Parser::equality() {
    Expression* expr = comparison();

    TokenType types[] {TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL};

    while (match(types))
    {
        Token token_operator{previous()};
        Expression* right = comparison();
        Binary *binary = new Binary{expr, token_operator, expr};
        expr = binary;
    }

    return expr;
}

Expression* Parser::comparison() {
    Expression* expr = term();

    TokenType types[] {TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL};

    while(match(types)) {
        Token token_operator{previous()};
        Expression* right = term();
        Binary *binary = new Binary{expr, token_operator, expr};
        expr = binary;
    }

    return expr;
}


bool Parser::match(const TokenType types[]) {
    for(int i = 0; i < sizeof(types) / sizeof(TokenType); i++ ) {
        if(m_tokens[m_current].m_token_type == types[i]) {
            m_current++;
            return true;
        } else {
            return false;
        }
    }
}

Token Parser::previous() {
    return m_tokens[m_current - 1];
}

Token Parser::advance() {
    if(!is_at_end) m_current++;
    return previous();
}

Token Parser::peek() {
    return m_tokens[m_current];
}

bool Parser::is_at_end() {
    return peek().m_token_type = TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) {
    if(is_at_end()) return false;

}

