#include "./resolver.h"

void Resolver::resolve(std::vector<Statement *> statements) {
    for (Statement *stmt : statements) {
        resolve(stmt);
    }
}

std::any Resolver::visit(Block *block) {
    beginScope();
    resolve(*block->m_statements);
    endScope(0);
    return nullptr;
}

std::any Resolver::visit(ClassDecl *classdecl) {
    ClassType enclosing_class = m_current_class;
    m_current_class = ClassType::CLASS;

    declare(classdecl->m_name);
    define(classdecl->m_name);

    beginScope();
    m_scopes.back()["this"] = std::pair<bool, bool>{true, true};

    for (Function *func : classdecl->m_methods)
    {
        FunctionType declaration = FunctionType::METHOD;
        if (func->m_name.m_lexeme == "init") declaration = FunctionType::INITIALIZER;
        resolveFunction(func, declaration);
    }

    for (Function *stat : classdecl->m_statics) {
        FunctionType declaration = FunctionType::STATIC;
        resolveFunction(stat, declaration);
    }

    for (Function *the_getter : classdecl->m_getters) {
        FunctionType declaration = FunctionType::GETTER;
        resolveFunction(the_getter, declaration);
    }

    endScope(classdecl->m_name.m_line);

    m_current_class = enclosing_class;
    return nullptr;
}

std::any Resolver::visit(Var *var) {
    declare(var->m_name);
    if(var->m_initializer != nullptr) {
        resolve(var->m_initializer);
    }
    define(var->m_name);
    return nullptr;
}

std::any Resolver::visit(Get *get) {
    resolve(get->m_object);
    return nullptr;
}

std::any Resolver::visit(Set *set) {
    resolve(set->m_value);
    resolve(set->m_object);
    return nullptr;
}

std::any Resolver::visit(ThisExpr *thisExpr) {
    if(m_current_class == ClassType::NONE)
        m_error_reporter->error(thisExpr->m_keyword, "Can't use 'this' outside class");
    resolveLocal(thisExpr, thisExpr->m_keyword);
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
        && m_scopes.back().find(variable->m_name.m_lexeme) != m_scopes.back().end() && m_scopes.back().at(variable->m_name.m_lexeme).first == false) {
            m_error_reporter->error(variable->m_name, "Can't read local variable of it's own initializer");
        }
    resolveLocal(variable, variable->m_name);
    return nullptr;
}

std::any Resolver::visit(Function *function) {
    declare(function->m_name);
    define(function->m_name);

    resolveFunction(function, FunctionType::FUNCTION);

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

std::any Resolver::visit(ReturnStmt *stmt) {
    if(m_current_function == FunctionType::NONE) {
        m_error_reporter->error(stmt->m_keyword, "Can't return outside a function.");
    }

    if(m_current_function == FunctionType::INITIALIZER) {
        m_error_reporter->error(stmt->m_keyword, "Can't return in constructor.");
    }

    if(stmt->m_value) {
        resolve(stmt->m_value);
    }

    return nullptr;
}

std::any Resolver::visit(Ternary *ternary) {
    resolve(ternary->m_condition);
    resolve(ternary->m_consequence);
    resolve(ternary->m_alternative);
    return nullptr;
}

std::any Resolver::visit(BreakStmt *breakStmt) {
    return nullptr;
}

std::any Resolver::visit(Binary *binary) {
    resolve(binary->m_left);
    resolve(binary->m_right);
    return nullptr;
}

std::any Resolver::visit(Call *call) {
    resolve(call->m_callee);

    for(Expression *arg : call->m_arguments) {
        resolve(arg);
    }

    return nullptr;
}

std::any Resolver::visit(Grouping *group) {
    resolve(group->m_expression);

    return nullptr;
}

std::any Resolver::visit(Literal *literal) {
    return nullptr;
}

std::any Resolver::visit(Logical *logical) {
    resolve(logical->m_left);
    resolve(logical->m_right);
    return nullptr;
}

std::any Resolver::visit(Unary *unary) {
    resolve(unary->m_right);
    return nullptr;
}

std::any Resolver::visit(WhileStmt *stmt) {
    resolve(stmt->m_condition);
    resolve(stmt->m_body);
    return nullptr;
}

void Resolver::resolveLocal(Expression *expr, Token name) {
    for (int i = m_scopes.size() - 1 ; i >= 0; i--)
    {
        if(m_scopes.at(i).find(name.m_lexeme) !=  m_scopes.at(i).end()) {
            m_evaluator->resolve(expr, m_scopes.size() - 1 - i);
            m_scopes.at(i).at(name.m_lexeme).second = true;
            return;
        }
    }
}

void Resolver::resolveFunction(Function *function, FunctionType type) {
    FunctionType enclosing_function = m_current_function;
    m_current_function = type;
    beginScope();
    for(Token param : function->m_params) {
        declare(param);
        define(param);
    }

    resolve(function->m_body);
    endScope(function->m_name.m_line);
    m_current_function = enclosing_function;
}

void Resolver::resolve(Statement *stmt) {
    stmt->accept(this);
}

void Resolver::resolve(Expression *expr) {
    expr->accept(this);
}

void Resolver::declare(Token name) {
    if(m_scopes.empty()) return;

    std::map<std::string, std::pair<bool, bool>> scope{m_scopes.back()};

    if(scope.find(name.m_lexeme) != scope.end()) {
        m_error_reporter->error(name, "Can't redeclare variable in the same scope");
    }

    scope[name.m_lexeme] = std::pair<bool, bool>{false, false};
}

void Resolver::define(Token name) {
    if(m_scopes.empty()) return;
    m_scopes.back()[name.m_lexeme].first = true;
}

void Resolver::beginScope() {
    m_scopes.push_back(std::map<std::string, std::pair<bool, bool>>{});
}

void Resolver::endScope(int line) {
    checkLocalVariableUsage(line);
    m_scopes.pop_back();
}

void Resolver::checkLocalVariableUsage(int line) {
    std::map<std::string, std::pair<bool, bool>> scope{m_scopes.back()};
    for (std::pair<std::string, std::pair<bool, bool>> variable : scope) {
        if(variable.second.second == false) {
            m_error_reporter->error(0, std::string{"Unused local variable " + variable.first}.c_str());
        }
    }
}