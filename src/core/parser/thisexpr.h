// this file has been auto generated
#ifndef THISEXPR_H
#define THISEXPR_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class ThisExpr : public Expression {
public:
 Token m_keyword;
ThisExpr( Token m_keyword) : m_keyword{m_keyword}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual ThisExpr* clone(){
return new ThisExpr(*this);
};
virtual ~ThisExpr(){};
};
#endif
