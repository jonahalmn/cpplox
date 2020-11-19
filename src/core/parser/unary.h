// this file has been auto generated
#ifndef UNARY_H
#define UNARY_H
#include <iostream>
#include <any>
#include "./type_defs.h"
#include "./expression.h"
#include "../lexer/token.h"
class Unary : public Expression {
public:
 Token m_operator;
 Expression *m_right;
Unary( Token m_operator, Expression *m_right) : m_operator{m_operator},m_right{m_right}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual ~Unary(){};
};
#endif
