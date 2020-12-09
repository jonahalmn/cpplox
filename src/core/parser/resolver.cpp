#include "./resolver.h"

void Resolver::resolve(std::vector<Statement *> statements) {
    for (Statement *stmt : statements) {
        resolve(stmt);
    }
}

std::any Resolver::visit(Block *block) {
    beginScope();
    resolve(*block->m_statements);
    endScope();
}

std::any Resolver::visit(Var *var) {
    declare(var->m_name);
    if(var->m_initializer != nullptr) {
        resolve(var->m_initializer);
    }
    define(var->m_name);
    return nullptr;
}

std::any Resolver::visit(Assign *assign) {
    resolve(assign->m_value);
    resolveLocal(assign, assign->m_name);
    return nullptr;
}

std::any Resolver::visit(StmtExpression* expr) {
    resolve(expr->m_expression);
    return nullptr;
}

std::any Resolver::visit(Variable *variable) {
    if(!m_scopes.empty()
        && m_scopes.back().find(variable->m_name.m_lexeme) == m_scopes.back().end()) {
            m_error_reporter->error(variable->m_name, "Can't read local variable of it's own initializer");
        }
    resolveLocal(variable, variable->m_name);
}

std::any Resolver::visit(Function *function) {
    declare(function->m_name);
    define(function->m_name);

    resolveFunction(function);

    return nullptr;
}

std::any Resolver::visit(Print *print) {
    resolve(print->m_expression);
    return nullptr;
}

std::any Resolver::visit(IfStmt *stmt) {
    resolve(stmt->m_condition);
    resolve(stmt->m_then);
    if(stmt->m_else != nullptr) resolve(stmt->m_else);
    return nullptr;
}

void Resolver::resolveLocal(Expression *expr, Token name) {
    for (int i = m_scopes.size() -1 ; i >= 0; i--)
    {
        if(m_scopes.at(i).find(name.m_lexeme) !=  m_scopes.at(i).end()) {
            // m_evaluator.resolve(expr, m_scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::resolveFunction(Function *function) {
    beginScope();
    for(Token param : function->m_params) {
        declare(param);
        define(param);
    }

    resolve(function->m_body);
    endScope();
}

void Resolver::resolve(Statement *stmt) {
    stmt->accept(this);
}

void Resolver::resolve(Expression *expr) {
    expr->accept(this);
}

void Resolver::declare(Token name) {
    if(m_scopes.empty()) return;

    std::map<std::string, bool> scope{m_scopes.back()};
    scope.insert(std::pair<std::string, bool>{name.m_lexeme, false});
}

void Resolver::define(Token name) {
    if(m_scopes.empty()) return;
    m_scopes.back().insert(std::pair<std::string, bool>{name.m_lexeme, true});
}

void Resolver::beginScope() {
    m_scopes.push_back(std::map<std::string, bool>{});
}

void Resolver::endScope() {
    m_scopes.pop_back();
}