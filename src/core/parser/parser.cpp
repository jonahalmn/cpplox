#include "./parser.h"

Expression* Parser::expression() {
    return equality();
}

Expression* Parser::equality() {
    Expression* expr = comparison();

    TokenType types[] {TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL};

    while (match(types))
    {
        Token token_operator;
        Expression* right = comparison();
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

}

bool Parser::is_at_end() {

}

