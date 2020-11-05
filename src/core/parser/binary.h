// this file has been auto generated
#ifndef BINARY_H
#define BINARY_H
#include <iostream>
#include "./type_defs.h"
#include "./expression.h"
#include "../lexer/token.h"
template<class T> class Binary : public Expression<T>{
public:
 Expression<T> *m_left;
 Token m_operator;
 Expression<T> *m_right;
Binary(){}
virtual T accept(Visitor<T> *visitor){
return visitor->visit(this);
};
virtual ~Binary(){};
};
#endif
