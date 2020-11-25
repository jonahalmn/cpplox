// this file has been auto generated
#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Block : public Statement {
public:
 std::vector<Statement *> *m_statements;
Block( std::vector<Statement *> *m_statements) : m_statements{m_statements}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Block* clone(){
return new Block(*this);
};
virtual ~Block(){};
};
#endif
