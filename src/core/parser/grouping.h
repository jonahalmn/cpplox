// this file has been auto generated
#ifndef GROUPING_H
#define GROUPING_H
#include <iostream>
#include "./type_defs.h"
#include "./expression.h"
#include "../lexer/token.h"
template<class T> class Grouping : public Expression<T>{
public:
 Expression<T> *m_expression;
Grouping(){}
virtual T accept(Visitor<T> *visitor){
return visitor->visit(this);
};
virtual ~Grouping(){};
};
#endif
