#include <iostream>
#include "./token.h"

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "Token: " << token.m_token_type << " " << token.m_lexeme << " " << token.m_line << std::endl;
    return os;
}