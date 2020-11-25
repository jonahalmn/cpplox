// this file has been auto generated
#ifndef PRINT_H
#define PRINT_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Print : public Statement {
public:
 Expression *m_expression;
Print( Expression *m_expression) : m_expression{m_expression}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Print* clone(){
return new Print(*this);
};
virtual ~Print(){};
};
#endif
