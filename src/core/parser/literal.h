// this file has been auto generated
#ifndef LITERAL_H
#define LITERAL_H
#include <iostream>
#include "./type_defs.h"
#include "./expression.h"
#include "../lexer/token.h"
template<class T> class Literal : public Expression<T>{
public:
 std::string m_value;
 std::string m_type;
Literal(){}
virtual T accept(Visitor<T> *visitor){
return visitor->visit(this);
};
virtual ~Literal(){};
};
#endif
