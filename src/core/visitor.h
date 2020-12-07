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
virtual std::any visit(Variable *variable) = 0;
virtual std::any visit(Assign *assign) = 0;
virtual std::any visit(Logical *logical) = 0;
virtual std::any visit(Call *call) = 0;
virtual std::any visit(StmtExpression *stmtexpression) = 0;
virtual std::any visit(Print *print) = 0;
virtual std::any visit(Var *var) = 0;
virtual std::any visit(Block *block) = 0;
virtual std::any visit(IfStmt *ifstmt) = 0;
virtual std::any visit(WhileStmt *whilestmt) = 0;
virtual std::any visit(BreakStmt *breakstmt) = 0;
virtual std::any visit(Function *function) = 0;
virtual ~Visitor(){};
};
#endif
