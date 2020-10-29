#include <iostream>
#ifndef ERROR_H
#define ERROR_H

class ErrorReporter {
    private:
        int m_had_error = 0;
        static ErrorReporter *m_instance;

        ErrorReporter() {};

    public:
        static ErrorReporter* getInstance();
        int had_error();
        void error(unsigned int line, const char* message);
        void report(unsigned int line, const char* where, const char* message);
};

#endif