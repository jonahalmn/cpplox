#include "file.h"
#include <string>

std::string File::read() {
    std::string line;
    std::string output;

    if(m_file.is_open()) {
        while(std::getline(m_file, line)) {
            output += line;
        }
    }

    return output;
}