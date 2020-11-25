// this file has been auto generated
#ifndef VARIABLE_H
#define VARIABLE_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Variable : public Expression {
public:
 Token m_name;
Variable( Token m_name) : m_name{m_name}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Variable* clone(){
return new Variable(*this);
};
virtual ~Variable(){};
};
#endif
