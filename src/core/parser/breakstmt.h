// this file has been auto generated
#ifndef BREAKSTMT_H
#define BREAKSTMT_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class BreakStmt : public Statement {
public:
 Token m_token;
BreakStmt( Token m_token) : m_token{m_token}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual BreakStmt* clone(){
return new BreakStmt(*this);
};
virtual ~BreakStmt(){};
};
#endif
