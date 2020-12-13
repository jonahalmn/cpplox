// this file has been auto generated
#ifndef SET_H
#define SET_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Set : public Expression {
public:
 Expression *m_object;
 Token m_name;
 Expression *m_value;
Set( Expression *m_object, Token m_name, Expression *m_value) : m_object{m_object},m_name{m_name},m_value{m_value}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Set* clone(){
return new Set(*this);
};
virtual ~Set(){};
};
#endif
