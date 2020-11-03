#include <iostream>
#include "./expression.h"
#include "../lexer/token.h"
class Binary : Expression{
 Expression m_left;
 Token m_operator;
 Expression m_right;
};
