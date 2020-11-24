#include <iostream>
#include "./lexer/token.h"
#include "./error/runtimeError.h"
#ifndef ERROR_H
#define ERROR_H

class ErrorReporter {
    private:
        int m_had_error = 0;
        int m_had_runtime_error = 0;
        static ErrorReporter *m_instance;

        ErrorReporter() {};

    public:
        static ErrorReporter* getInstance();
        int had_error();
        int had_runtime_error();
        void error(unsigned int line, const char* message);
        void error(Token token, const char* message);
        void error(RuntimeError e);
        void report(unsigned int line, const char* where, const char* message);
};

#endif