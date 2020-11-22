#include <cctype>
#include "./scanner.h"
#include "../../inc/token_types.h"

std::vector<Token> Scanner::get_tokens() {
    return m_list;
}

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
    m_start = m_current;
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
        case '?': add_token(TokenType::QUESTION_MARK); break;
        case ':': add_token(TokenType::COLON); break;
        case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        case '/': if( match('/') ) {
            while (peek() != '\n' && !is_at_end()) advance();
        } else if ( match('*') ) {
            while (peek() != '*' && peek_next() != '/' && !is_at_end()) {
                if (peek() == '\n') m_line++;
                advance();
            };
            if (is_at_end()) {
                m_error_reporter->error(m_line, "Unexpected end of comment");
            } else {
                advance(2);
            }
        } else {
            add_token(TokenType::SLASH);
        }
        break;
        case '\n':
            m_line++;
            break;
        case ' ':
        case '\r':
        case '\t':
        break;
        case '"': string(); break;

    default:
        if (isdigit(c)) {
            number();
            break;
        }else if(isalpha(c)) {
            identifier();
            break;
        }

        m_error_reporter->error(m_line, "Unexpected character");
        break;
    }
}

void Scanner::add_token(TokenType type) {
    std::string text = m_source.substr(m_start, m_current - m_start);
    m_list.push_back(Token(type, text.c_str(), m_line));
}

void Scanner::add_token(TokenType type, const char* text) {
    m_list.push_back(Token(type, text, m_line));
}

void Scanner::identifier() {
    while(isalnum(peek())) advance();

    std::string text = m_source.substr(m_start, m_current - m_start);
    TokenType type;

    std::cout << "The text:" << text << std::endl;

    if(keywords.find(text) == keywords.end()) {
        type = TokenType::IDENTIFIER;
    }
    else {
        type = keywords.at(text);
    }

    add_token(type);
}

void Scanner::string() {
    while(peek() != '"' && !is_at_end()) {
        if(peek() == '\n') m_line++;

        advance();
    }

    if (is_at_end()) {
        m_error_reporter->error(m_line, "Unexpected end of string");
    }

    advance();

    std::string value = m_source.substr(m_start + 1, (m_current - m_start) - 2);
    add_token(TokenType::STRING, value.c_str());
}

void Scanner::number() {
    while (isdigit(peek()) && !is_at_end()) advance();
    if (peek() == '.' && isdigit(peek_next())) {
        advance();
        while (isdigit(peek()) && !is_at_end()) advance();
    }

    add_token(TokenType::NUMBER);

}

char Scanner::peek_next() {
    if(m_current + 1 >= m_source.length()) return '\0';
    return m_source.at(m_current + 1);
}

char Scanner::advance(unsigned int step) {
    m_current += step;
    return m_source.at(m_current - 1);
}

char Scanner::peek() {
    if (is_at_end()) return '\0';
    return m_source.at(m_current);
}

bool Scanner::match(char expected) {
    if(is_at_end()) return false;
    if(m_source.at(m_current) != expected) return false;

    m_current++;
    return true;
}