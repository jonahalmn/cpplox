// this file has been auto generated
#ifndef GROUPING_H
#define GROUPING_H
#include <iostream>
#include <any>
#include "./type_defs.h"
#include "./expression.h"
#include "../lexer/token.h"
class Grouping : public Expression {
public:
 Expression *m_expression;
Grouping( Expression *m_expression) : m_expression{m_expression}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual ~Grouping(){};
};
#endif
