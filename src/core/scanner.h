#include <iostream>
#include <vector>
#include "./token.h"
#include "./errorReporter.h"

#ifndef SCANNER_H
#define SCANNER_H

class Scanner {
    private:
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
        char advance();
        void add_token(TokenType type);
        void scan_token();
};

#endif