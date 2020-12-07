#include "./types.h"
#include <iostream>
#include <sstream>
#include <any>
#include <string>
#include <vector>
#include "../../helpers/helpers.h"
#include "../error/runtimeError.h"
#include "../errorReporter.h"
#include "../visitor.h"
#include "../parser/statement.h"
#include "../parser/literal.h"
#include "../parser/grouping.h"
#include "../parser/unary.h"
#include "../parser/binary.h"
#include "../parser/ternary.h"
#include "../parser/stmtexpression.h"
#include "../parser/print.h"
#include "../parser/var.h"
#include "../parser/variable.h"
#include "../parser/assign.h"
#include "../parser/block.h"
#include "../parser/ifstmt.h"
#include "../parser/logical.h"
#include "../parser/whilestmt.h"
#include "../parser/breakstmt.h"
#include "../parser/call.h"
#include "../parser/function.h"
#include "./loxFunction.h"


#include "./environment.h"
#include "./loxCallable.h"
#include "./loxFunction.h"
#include "./clock.h"

#ifndef EVAL_H
#define EVAL_H


class Evaluator : public Visitor {
    public:
        bool m_has_runtime_error = false;
        ErrorReporter *m_error_reporter = ErrorReporter::getInstance();
        Environment *m_global = new Environment{};
        Environment *m_environment = m_global;

        Evaluator();

        std::any interpret(std::vector<Statement *>);
        std::any evaluate(Expression*);
        std::any execute(Statement*);
        std::any executeBlock(Block*, Environment*);

        std::string stringify(std::any);
        void runtimeError(RuntimeError);

        bool isTruthy(std::any expr);
        bool isEqual(std::any, std::any);

        virtual std::any visit(Literal *literal);
        virtual std::any visit(Grouping *grouping);
        virtual std::any visit(Unary *unary);
        virtual std::any visit(Binary *binary);
        virtual std::any visit(Ternary *ternary);

        virtual std::any visit(StmtExpression *stmtExpr);
        virtual std::any visit(Print *print);

        virtual std::any visit(Var *var);
        virtual std::any visit(Variable *variable);
        virtual std::any visit(Assign *assign);

        virtual std::any visit(IfStmt *ifStmt);
        virtual std::any visit(WhileStmt *whileStmt);
        virtual std::any visit(Logical *logical);
        virtual std::any visit(BreakStmt *breakstmt);

        virtual std::any visit(Call *call);

        virtual std::any visit(Function *func);

        virtual std::any visit(Block *block);

        void checkNumberOperand(Token token, std::any operand);
        void checkNumberOperands(Token token, std::any left, std::any right);
};

#endif