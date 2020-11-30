// this file has been auto generated
#ifndef IFSTMT_H
#define IFSTMT_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class IfStmt : public Statement {
public:
 Expression* m_condition;
 Statement *m_then;
 Statement *m_else;
IfStmt( Expression* m_condition, Statement *m_then, Statement *m_else) : m_condition{m_condition},m_then{m_then},m_else{m_else}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual IfStmt* clone(){
return new IfStmt(*this);
};
virtual ~IfStmt(){};
};
#endif
