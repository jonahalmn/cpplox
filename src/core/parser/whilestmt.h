// this file has been auto generated
#ifndef WHILESTMT_H
#define WHILESTMT_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class WhileStmt : public Statement {
public:
 Expression *m_condition;
 Statement *m_body;
WhileStmt( Expression *m_condition, Statement *m_body) : m_condition{m_condition},m_body{m_body}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual WhileStmt* clone(){
return new WhileStmt(*this);
};
virtual ~WhileStmt(){};
};
#endif
