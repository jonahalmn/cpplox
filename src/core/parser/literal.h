// this file has been auto generated
#ifndef LITERAL_H
#define LITERAL_H
#include <iostream>
#include <any>
#include "./type_defs.h"
#include "./expression.h"
#include "../lexer/token.h"
class Literal : public Expression {
public:
 std::string m_value;
 std::string m_type;
Literal( std::string m_value, std::string m_type){}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual ~Literal(){};
};
#endif
