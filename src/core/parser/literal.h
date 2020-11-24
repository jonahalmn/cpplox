// this file has been auto generated
#ifndef LITERAL_H
#define LITERAL_H
#include <iostream>
#include <any>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Literal : public Expression {
public:
 std::any m_value;
Literal( std::any m_value) : m_value{m_value}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Literal* clone(){
return new Literal(*this);
};
virtual ~Literal(){};
};
#endif
