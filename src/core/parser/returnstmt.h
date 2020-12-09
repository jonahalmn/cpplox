// this file has been auto generated
#ifndef RETURNSTMT_H
#define RETURNSTMT_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class ReturnStmt : public Statement {
public:
 Token m_keyword;
 Expression *m_value;
ReturnStmt( Token m_keyword, Expression *m_value) : m_keyword{m_keyword},m_value{m_value}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual ReturnStmt* clone(){
return new ReturnStmt(*this);
};
virtual ~ReturnStmt(){};
};
#endif
