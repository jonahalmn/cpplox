#ifndef VISITOR_H
#define VISITOR_H
// this file has been auto generated
#include "./parser/type_defs.h"
class Visitor {
public:
virtual std::any visit(Binary *binary) = 0;
virtual std::any visit(Grouping *grouping) = 0;
virtual std::any visit(Literal *literal) = 0;
virtual std::any visit(Unary *unary) = 0;
virtual std::any visit(Ternary *ternary) = 0;
virtual std::any visit(StmtExpression *stmtexpression) = 0;
virtual std::any visit(Print *print) = 0;
virtual ~Visitor(){};
};
#endif
