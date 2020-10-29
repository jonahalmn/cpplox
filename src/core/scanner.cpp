#include "./scanner.h"
#include "../inc/token_types.h"

void Scanner::add_source(std::string source) {
    m_source = source;
}

void Scanner::scan_tokens() {
    while (!is_at_end())
    {
        scan_token();
    }

    m_list.push_back(Token(TokenType::END_OF_FILE, "", m_line));

    for (auto &token : m_list) // access by reference to avoid copying
    {
        std::cout << token << std::endl;
    }
}


bool Scanner::is_at_end() {
    return m_source.length() <= m_current;
}

void Scanner::scan_token() {
    char c = advance();
    switch (c)
    {
        case '(': add_token(TokenType::LEFT_PAREN); break;
        case ')': add_token(TokenType::RIGHT_PAREN); break;
        case '{': add_token(TokenType::LEFT_BRACE); break;
        case '}': add_token(TokenType::RIGHT_BRACE); break;
        case ',': add_token(TokenType::COMMA); break;
        case '.': add_token(TokenType::DOT); break;
        case '-': add_token(TokenType::MINUS); break;
        case '+': add_token(TokenType::PLUS); break;
        case ';': add_token(TokenType::SEMICOLON); break;
        case '*': add_token(TokenType::STAR); break;
    default:
        m_error_reporter->error(m_line, "Unexpecter character");
        break;
    }
}

void Scanner::add_token(TokenType type) {
    std::string text = m_source.substr(m_start, m_current - m_start);
    m_list.push_back(Token(type, text.c_str(), m_line));
}

char Scanner::advance() {
    m_current++;
    return m_source.at(m_current - 1);
}