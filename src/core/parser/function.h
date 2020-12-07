// this file has been auto generated
#ifndef FUNCTION_H
#define FUNCTION_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Function : public Statement {
public:
 Token m_name;
 std::vector<Token> m_params;
 Block *m_body;
Function( Token m_name, std::vector<Token> m_params, Block *m_body) : m_name{m_name},m_params{m_params},m_body{m_body}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Function* clone(){
return new Function(*this);
};
virtual ~Function(){};
};
#endif
