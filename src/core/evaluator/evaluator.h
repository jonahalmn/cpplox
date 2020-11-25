#include <iostream>
#include <any>
#include <string>
#include <vector>
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

#include "./environment.h"

#ifndef EVAL_H
#define EVAL_H

class Evaluator : public Visitor {
    public:
        bool m_has_runtime_error = false;
        ErrorReporter *m_error_reporter = ErrorReporter::getInstance();

        Environment *m_environment = new Environment{};

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

        virtual std::any visit(Block *block);

        void checkNumberOperand(Token token, std::any operand);
        void checkNumberOperands(Token token, std::any left, std::any right);
};

#endif