// this file has been auto generated
#ifndef SUPEREXPR_H
#define SUPEREXPR_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class SuperExpr : public Expression {
public:
 Token m_keyword;
 Token m_method;
SuperExpr( Token m_keyword, Token m_method) : m_keyword{m_keyword},m_method{m_method}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual SuperExpr* clone(){
return new SuperExpr(*this);
};
virtual ~SuperExpr(){};
};
#endif
