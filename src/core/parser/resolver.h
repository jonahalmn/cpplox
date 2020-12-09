#include <map>
#include <vector>
#include "../visitor.h"
#include "../evaluator/evaluator.h"
#include "../errorReporter.h"


class Resolver : public Visitor {
    public:
        Evaluator *m_evaluator;
        ErrorReporter *m_error_reporter = ErrorReporter::getInstance();

        std::vector<std::map<std::string, bool>> m_scopes;

        void beginScope();
        void endScope();

        void resolve(std::vector<Statement *>);
        void resolve(Statement *);
        void resolve(Expression *);

        void resolveLocal(Expression* expr, Token name);
        void resolveFunction(Function *);

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
        virtual std::any visit(Block *block);
        virtual std::any visit(IfStmt *ifstmt);
        virtual std::any visit(WhileStmt *whilestmt);
        virtual std::any visit(BreakStmt *breakstmt);
        virtual std::any visit(Function *function);
        virtual std::any visit(ReturnStmt *returnstmt);


        Resolver(Evaluator *evaluator) : m_evaluator{evaluator} {}
};