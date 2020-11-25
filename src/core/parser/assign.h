// this file has been auto generated
#ifndef ASSIGN_H
#define ASSIGN_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Assign : public Expression {
public:
 Token m_name;
 Expression *m_value;
Assign( Token m_name, Expression *m_value) : m_name{m_name},m_value{m_value}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Assign* clone(){
return new Assign(*this);
};
virtual ~Assign(){};
};
#endif
