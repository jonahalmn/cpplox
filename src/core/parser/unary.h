// this file has been auto generated
#ifndef UNARY_H
#define UNARY_H
#include <iostream>
#include "./type_defs.h"
#include "./expression.h"
#include "../lexer/token.h"
template<class T> class Unary : public Expression<T>{
public:
 Token m_operator;
 Expression<T> *m_right;
Unary(){}
virtual T accept(Visitor<T> *visitor){
return visitor->visit(this);
};
virtual ~Unary(){};
};
#endif
