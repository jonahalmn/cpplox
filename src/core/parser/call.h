// this file has been auto generated
#ifndef CALL_H
#define CALL_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Call : public Expression {
public:
 Expression *m_callee;
 Token m_parenthesis;
 std::vector<Expression *> m_arguments;
Call( Expression *m_callee, Token m_parenthesis, std::vector<Expression *> m_arguments) : m_callee{m_callee},m_parenthesis{m_parenthesis},m_arguments{m_arguments}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Call* clone(){
return new Call(*this);
};
virtual ~Call(){};
};
#endif
