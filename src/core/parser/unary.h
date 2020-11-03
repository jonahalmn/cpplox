#include <iostream>
#include "./expression.h"
#include "../lexer/token.h"
class Unary : Expression{
 Token m_operator;
 Expr m_right;
};
