#include <iostream>
#include <fstream>

#ifndef FILE_H
#define FILE_H

class File {
    private:
        std::ifstream m_file;

    public:
        File(const char* filename) : m_file{filename} {

        }


        ~File() {
            m_file.close();
        }

        std::string read();
};

#endif