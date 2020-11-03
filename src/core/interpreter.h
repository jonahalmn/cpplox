#include <iostream>
#include <string>
#include <vector>
#include "../fs/file.h"
#include "./lexer/token.h"
#include "./lexer/scanner.h"
#include "./errorReporter.h"


#ifndef INTERPRETER_H
#define INTERPRETER_H


class Interpreter {

    private:
        ErrorReporter *m_error_reporter = ErrorReporter::getInstance();
        std::string m_string;
        std::vector<Token> m_tokens;
        Scanner m_scanner;

    public:
        Interpreter(File* file) {
            m_string = file->read();
            run();
        }

        Interpreter(std::string string) : m_string{string} {
            run();
        }

        Interpreter(const char* filename) {
            File file{filename};
            m_string = file.read();
            run();
        }

        void run();
};

#endif