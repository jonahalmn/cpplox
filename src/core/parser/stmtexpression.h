// this file has been auto generated
#ifndef STMTEXPRESSION_H
#define STMTEXPRESSION_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class StmtExpression : public Statement {
public:
 Expression *m_expression;
StmtExpression( Expression *m_expression) : m_expression{m_expression}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual StmtExpression* clone(){
return new StmtExpression(*this);
};
virtual ~StmtExpression(){};
};
#endif
