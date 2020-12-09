// this file has been auto generated
#ifndef RETURN_H
#define RETURN_H
#include <iostream>
#include <any>
#include <vector>
#include "./type_defs.h"
#include "./expression.h"
#include "./statement.h"
#include "../lexer/token.h"
class Return : public Statement {
public:
 Token m_keyword;
 Expression *m_value;
Return( Token m_keyword, Expression *m_value) : m_keyword{m_keyword},m_value{m_value}{}
virtual std::any accept(Visitor *visitor){
return visitor->visit(this);
};
virtual Return* clone(){
return new Return(*this);
};
virtual ~Return(){};
};
#endif
