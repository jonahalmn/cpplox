// this file has been auto generated
#ifndef GET_H
#define GET_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Get : public Expression {
public:
 Expression *m_object;
 Token m_name;
Get( Expression *m_object, Token m_name) : m_object{m_object},m_name{m_name}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Get* clone(){
return new Get(*this);
};
virtual ~Get(){};
};
#endif
