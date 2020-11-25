// this file has been auto generated
#ifndef VAR_H
#define VAR_H
#include <iostream>
#include <any>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Var : public Statement {
public:
 Token m_name;
 Expression *m_initializer;
Var( Token m_name, Expression *m_initializer) : m_name{m_name},m_initializer{m_initializer}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Var* clone(){
return new Var(*this);
};
virtual ~Var(){};
};
#endif
