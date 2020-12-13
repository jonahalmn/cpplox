#include <map>
#include <vector>
#include <any>
#include "../visitor.h"
#include "../evaluator/evaluator.h"
#include "../errorReporter.h"
#include "../../inc/function_type.h"


class Resolver : public Visitor {
    public:
        Evaluator *m_evaluator;
        ErrorReporter *m_error_reporter = ErrorReporter::getInstance();

        std::vector<std::map<std::string, std::pair<bool, bool>>> m_scopes{};
        FunctionType m_current_function = NONE;

        void beginScope();
        void endScope(int line);

        void resolve(std::vector<Statement *>);
        void resolve(Statement *);
        void resolve(Expression *);

        void checkLocalVariableUsage(int line);

        void resolveLocal(Expression* expr, Token name);
        void resolveFunction(Function *, FunctionType);

        void declare(Token name);
        void define(Token name);

        virtual std::any visit(Binary *binary);
        virtual std::any visit(Grouping *grouping);
        virtual std::any visit(Literal *literal);
        virtual std::any visit(Unary *unary);
        virtual std::any visit(Ternary *ternary);
        virtual std::any visit(Variable *variable);
        virtual std::any visit(Assign *assign);
        virtual std::any visit(Logical *logical);
        virtual std::any visit(Call *call);
        virtual std::any visit(StmtExpression *stmtexpression);
        virtual std::any visit(Print *print);
        virtual std::any visit(Var *var);
        virtual std::any visit(Get *get);
        virtual std::any visit(Set *set);
        virtual std::any visit(ThisExpr *thisExpr);
        virtual std::any visit(Block *block);
        virtual std::any visit(IfStmt *ifstmt);
        virtual std::any visit(WhileStmt *whilestmt);
        virtual std::any visit(BreakStmt *breakstmt);
        virtual std::any visit(Function *function);
        virtual std::any visit(ReturnStmt *returnstmt);
        virtual std::any visit(ClassDecl *classdecl);


        Resolver(Evaluator *evaluator) : m_evaluator{evaluator} {}
};