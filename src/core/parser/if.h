// this file has been auto generated
#ifndef IF_H
#define IF_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class If : public Statement {
public:
 Expression* m_condition;
 Statement *m_then;
 Statement *m_else;
If( Expression* m_condition, Statement *m_then, Statement *m_else) : m_condition{m_condition},m_then{m_then},m_else{m_else}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual If* clone(){
return new If(*this);
};
virtual ~If(){};
};
#endif
