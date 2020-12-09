#include<any>

#ifndef RETURN_EXP
#define RETURN_EXP

class ReturnExeption {
    public:
        std::any m_value;

        ReturnExeption(std::any value) : m_value{value} {}
};

#endif