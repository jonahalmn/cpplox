#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "./token.h"
#include "../errorReporter.h"

#ifndef SCANNER_H
#define SCANNER_H

class Scanner {
    private:
        std::map<std::string, TokenType> keywords = {
            { "and", TokenType::AND },
            { "class", TokenType::CLASS },
            { "else", TokenType::ELSE },
            { "false", TokenType::FALSE },
            { "for", TokenType::FOR },
            { "fun", TokenType::FUN },
            { "if", TokenType::IF },
            { "nil", TokenType::NIL },
            { "or", TokenType::OR },
            { "print", TokenType::PRINT },
            { "return", TokenType::RETURN },
            { "super", TokenType::SUPER },
            { "this", TokenType::THIS },
            { "true", TokenType::TRUE },
            { "var", TokenType::VAR },
            { "while", TokenType::WHILE },
            { "break", TokenType::BREAK },
        };
        std::string m_source;
        std::vector<Token> m_list;
        ErrorReporter *m_error_reporter = ErrorReporter::getInstance();

        unsigned int m_start = 0;
        unsigned int m_current = 0;
        unsigned int m_line = 1;

    public:
        Scanner() {};
        Scanner(std::string source) : m_source{source} {};

        void add_source(std::string source);
        void scan_tokens();
        bool is_at_end();
        char advance(unsigned int step = 1);
        void add_token(TokenType type);
        void add_token(TokenType type, const char *text);
        void scan_token();
        bool match(char c);
        char peek();
        char peek_next();
        void string();
        void number();
        void identifier();

        std::vector<Token> get_tokens();

};

#endif