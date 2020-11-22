// this file has been auto generated
#ifndef TERNARY_H
#define TERNARY_H
#include <iostream>
#include <any>
#include "./type_defs.h"
#include "./expression.h"
#include "../lexer/token.h"
class Ternary : public Expression {
public:
 Expression *m_condition;
 Expression *m_consequence;
 Expression *m_alternative;
Ternary( Expression *m_condition, Expression *m_consequence, Expression *m_alternative) : m_condition{m_condition},m_consequence{m_consequence},m_alternative{m_alternative}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Ternary* clone(){
return new Ternary(*this);
};
virtual ~Ternary(){};
};
#endif
