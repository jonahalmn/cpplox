#include <iostream>
#include <any>
#include <string>
#include "../error/runtimeError.h"
#include "../parser/visitor.h"
#include "../parser/literal.h"
#include "../parser/grouping.h"
#include "../parser/unary.h"
#include "../parser/binary.h"
#include "../parser/ternary.h"

class Evaluator : public Visitor {
    public:
        std::any interpret(Expression*);
        std::any evaluate(Expression*);

        std::string stringify(std::any);

        bool isTruthy(std::any expr);
        bool isEqual(std::any, std::any);

        virtual std::any visit(Literal *literal);
        virtual std::any visit(Grouping *grouping);
        virtual std::any visit(Unary *unary);
        virtual std::any visit(Binary *binary);
        virtual std::any visit(Ternary *ternary);

        void checkNumberOperand(Token token, std::any operand);
        void checkNumberOperands(Token token, std::any left, std::any right);
};