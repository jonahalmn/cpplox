// this file has been auto generated
#ifndef CLASSDECL_H
#define CLASSDECL_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class ClassDecl : public Statement {
public:
 Token m_name;
 std::vector<Function *> m_methods;
ClassDecl( Token m_name, std::vector<Function *> m_methods) : m_name{m_name},m_methods{m_methods}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual ClassDecl* clone(){
return new ClassDecl(*this);
};
virtual ~ClassDecl(){};
};
#endif
