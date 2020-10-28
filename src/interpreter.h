#include <iostream>
#include <string>
#include "./file.h"

class Interpreter {

    private:
        std::string m_string;

    public:
        Interpreter(File* file) {
            m_string = file->read();
            run();
        }

        Interpreter(std::string string) : m_string{string} {
            run();
        }

        void run();

};