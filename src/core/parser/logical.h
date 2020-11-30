// this file has been auto generated
#ifndef LOGICAL_H
#define LOGICAL_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Logical : public Expression {
public:
 Expression *m_left;
 Token m_operator;
 Expression *m_right;
Logical( Expression *m_left, Token m_operator, Expression *m_right) : m_left{m_left},m_operator{m_operator},m_right{m_right}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Logical* clone(){
return new Logical(*this);
};
virtual ~Logical(){};
};
#endif
