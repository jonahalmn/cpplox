// this file has been auto generated
#ifndef BINARY_H
#define BINARY_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Binary : public Expression {
public:
 Expression *m_left;
 Token m_operator;
 Expression *m_right;
Binary( Expression *m_left, Token m_operator, Expression *m_right) : m_left{m_left},m_operator{m_operator},m_right{m_right}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Binary* clone(){
return new Binary(*this);
};
virtual ~Binary(){};
};
#endif
