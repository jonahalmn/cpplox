#include "./evaluator.h"

Evaluator::Evaluator() {
    LoxCallable *clockFun = new Clock{};
    m_global->define(Token{TokenType::IDENTIFIER, "clock", 0}, clockFun);
}

void Evaluator::resolve(Expression *expr, int depth) {
    m_locals[expr] = depth;
    return;
}

std::any Evaluator::visit(Literal *literal) {
    return literal->m_value;
}

std::any Evaluator::visit(Grouping *grouping) {
    return evaluate(grouping->m_expression);
}

std::any Evaluator::visit(BreakStmt *breakstmt) {
    throw new BreakStmt(breakstmt->m_token);
}

std::any Evaluator::visit(Unary *unary) {
    std::any right = evaluate(unary->m_right);

    switch (unary->m_operator.m_token_type)
    {
    case TokenType::MINUS:
        checkNumberOperand(unary->m_operator, right);
        return -std::any_cast<double>(right);
        break;
    case TokenType::BANG:
        return !isTruthy(right);
        break;
    default:
        break;
    }

    return NULL;
}

std::any Evaluator::visit(ReturnStmt *returnStmt) {
    std::any value = nullptr;
    if(returnStmt->m_value != nullptr) value = evaluate(returnStmt->m_value);
    throw new ReturnExeption{value};
}

std::any Evaluator::visit(Get *get) {
    std::any object = evaluate(get->m_object);

    try {
        if(object.type() == typeid(LoxInstance *)) {
            LoxInstance* instance = std::any_cast<LoxInstance *>(object);
            return instance->get(get->m_name, this);
        } else {
            LoxClass* instance = (LoxClass *) std::any_cast<LoxCallable *>(object);
            return instance->get(get->m_name);
        }
    } catch(std::bad_any_cast e) {
        m_error_reporter->error(RuntimeError{get->m_name, "trying to access property of non-object"});
    } catch( NonStaticError e ) {
        m_error_reporter->error(RuntimeError{get->m_name, "trying to access non-static property"});
    }

    return nullptr;
}

std::any Evaluator::visit(Set *set) {
    std::any object = evaluate(set->m_object);
    LoxInstance *object_instance;

    try {
        object_instance = std::any_cast<LoxInstance *>(object);
    } catch(std::bad_any_cast e) {
        throw RuntimeError{set->m_name, "trying to access field of no-object"};
    } catch(ForbiddenStaticSet e) {
        throw RuntimeError{set->m_name, "trying to set static field"};
    }

    std::any value = evaluate(set->m_value);
    object_instance->set(set->m_name, value);
    return value;
}

std::any Evaluator::visit(Binary *binary) {
    std::any right = evaluate(binary->m_right);
    std::any left = evaluate(binary->m_left);

    switch (binary->m_operator.m_token_type)
    {
    case TokenType::STAR:
        checkNumberOperands(binary->m_operator, left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
        break;
    case TokenType::SLASH:
        checkNumberOperands(binary->m_operator, left, right);
        if(std::any_cast<double>(right) == 0) throw RuntimeError(binary->m_operator, "You should go in maths class, can't divide by 0");
        return std::any_cast<double>(left) / std::any_cast<double>(right);
        break;
    case TokenType::MINUS:
        checkNumberOperands(binary->m_operator, left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);
        break;
    case TokenType::GREATER:
        checkNumberOperands(binary->m_operator, left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);
        break;
    case TokenType::GREATER_EQUAL:
        checkNumberOperands(binary->m_operator, left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
        break;
    case TokenType::LESS:
        checkNumberOperands(binary->m_operator, left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);
        break;
    case TokenType::LESS_EQUAL:
        checkNumberOperands(binary->m_operator, left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);
        break;
    case TokenType::PLUS:
        if(left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        } else if (left.type() == typeid(double) && right.type() == typeid(double)) {
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        } else if (left.type() == typeid(std::string) && right.type() == typeid(double)) {
            return std::any_cast<std::string>(left) + std::to_string(std::any_cast<double>(right));
        } else if (left.type() == typeid(double) && right.type() == typeid(std::string)) {
            return std::to_string(std::any_cast<double>(left)) + std::any_cast<std::string>(right);
        }

        throw RuntimeError(binary->m_operator, "Operands must be two numbers or two strings.");
        break;
    case TokenType::BANG_EQUAL:
        return !isEqual(left, right);
        break;
    case TokenType::EQUAL_EQUAL:
        return isEqual(left, right);
        break;
    default:
        throw RuntimeError(binary->m_operator, "couldn't understand operator");
        return 1;
        break;
    }
}

std::any Evaluator::visit(Ternary *ternary) {
    std::any condition = evaluate(ternary->m_condition);
    std::any consequence = evaluate(ternary->m_consequence);
    std::any alternative = evaluate(ternary->m_alternative);

    return isTruthy(condition) ? consequence : alternative;
}

std::any Evaluator::visit(StmtExpression *stmtExpr) {
    evaluate(stmtExpr->m_expression);
    return nullptr;
}

std::any Evaluator::visit(Print *print) {
    std::any value = evaluate(print->m_expression);
    std::cout << stringify(value) << std::endl;
    return nullptr;
}

void Evaluator::checkNumberOperand(Token token, std::any operand) {
    if(operand.type() == typeid(double)) return;

    throw RuntimeError(token, "operand must be a number");
}

void Evaluator::checkNumberOperands(Token token, std::any left, std::any right) {
    if(left.type() == typeid(double) && right.type() == typeid(double)) return;

    throw RuntimeError(token, "operands must be numbers");
}

std::any Evaluator::visit(Var *var) {
    std::any value = nullptr;
    if(var->m_initializer != nullptr) {
        value = evaluate(var->m_initializer);
    }

    m_environment->define(var->m_name, value);
    return nullptr;
}

std::any Evaluator::visit(ClassDecl *classdecl) {
    std::any superclass = nullptr;
    LoxClass *superclass_class = nullptr;

    if(classdecl->m_superclass) {
        superclass = evaluate(classdecl->m_superclass);
        if(typeid(LoxCallable *) != superclass.type()){
            throw RuntimeError(classdecl->m_name, "superclass is not a class");
        }

        LoxCallable *superclass_callable = std::any_cast<LoxCallable *>(superclass);
        superclass_class = dynamic_cast<LoxClass *>(superclass_callable);

        if(!superclass_class) {
            throw RuntimeError(classdecl->m_name, "superclass is not a class");
        }
    }

    m_environment->define(classdecl->m_name, nullptr);

    if(classdecl->m_superclass) {
        m_environment = new Environment{m_environment};
        m_environment->define(
            Token{TokenType::SUPER, "super", classdecl->m_name.m_line},
            superclass_class
        );
    }

    std::map<std::string, LoxCallable *> methods {};
    std::map<std::string, LoxCallable *> statics {};
    std::map<std::string, LoxCallable *> getters {};

    for (Function *method : classdecl->m_methods)
    {
        LoxCallable *function = new LoxFunction{method, m_environment, method->m_name.m_lexeme == "init"};
        methods[method->m_name.m_lexeme] = function;
    }

    for (Function *stat : classdecl->m_statics)
    {
        LoxCallable *function = new LoxFunction{stat, m_environment, false};
        statics[stat->m_name.m_lexeme] = function;
    }

    for (Function *get : classdecl->m_getters)
    {
        LoxCallable *function = new LoxFunction{get, m_environment, false};
        getters[get->m_name.m_lexeme] = function;
    }

    LoxCallable *klass = new LoxClass{
        classdecl->m_name,
        superclass_class,
        methods,
        statics,
        getters
    };

    if(classdecl->m_superclass) {
        m_environment = m_environment->m_enclosing;
    }

    m_environment->assign(classdecl->m_name, klass);

    return nullptr;
}

std::any Evaluator::visit(SuperExpr *superExpr) {
    int distance = m_locals.at(superExpr);
    LoxClass *superclass = std::any_cast<LoxClass *>(m_environment->get_at(distance, "super"));

    LoxInstance *object = std::any_cast<LoxInstance *>(m_environment->get_at(distance - 1, "this"));

    LoxFunction *method = (LoxFunction *) superclass->findMethod(superExpr->m_method.m_lexeme);

    if(!method) {
        throw RuntimeError{superExpr->m_keyword, "trying to access to undefined property"};
    }

    return (LoxCallable *) method->bind(object);
}

std::any Evaluator::visit(ThisExpr *thisExpr) {
    return lookup_variable(thisExpr->m_keyword, thisExpr);
}

std::any Evaluator::lookup_variable(Token token, Expression *expr) {
    if(m_locals.find(expr) != m_locals.end()) {
        int distance = m_locals.at(expr);
        return m_environment->get_at(distance, token.m_lexeme);
    } else {
        return m_global->get(token);
    }
}

std::any Evaluator::visit(Variable *variable) {
    return lookup_variable(variable->m_name, variable);
}

std::any Evaluator::visit(Block *block) {
    Environment *environment = new Environment{m_environment};
    executeBlock(block, environment);
    return nullptr;
}

std::any Evaluator::visit(Assign *assign) {
    std::any value = evaluate(assign->m_value);

    if(m_locals.find(assign) != m_locals.end()) {
        int distance = m_locals.at(assign);
        return m_environment->assign_at(distance, assign->m_name, value);
    } else {
        return m_global->assign(assign->m_name, value);
    }

    return value;
}

std::any Evaluator::visit(IfStmt *ifStmt) {
    if(isTruthy(evaluate(ifStmt->m_condition))) {
        return execute(ifStmt->m_then);
    } else if(ifStmt->m_else) {
        return execute(ifStmt->m_else);
    }

    return nullptr;
}

std::any Evaluator::visit(WhileStmt *whileStmt) {
    try {
        while(isTruthy(evaluate(whileStmt->m_condition))) {
            execute(whileStmt->m_body);
        }
    } catch(BreakStmt *e) {
        return nullptr;
    }

    return nullptr;
}

std::any Evaluator::visit(Call *call) {
    std::any callee = evaluate(call->m_callee);

    std::vector<std::any> arguments{};
    for(Expression *argument : call->m_arguments) {
        arguments.push_back(evaluate(argument));
    }
    LoxCallable *functionCallable;
    try {
        functionCallable = std::any_cast<LoxCallable *>(callee);
    } catch (const std::bad_any_cast& e) {
        throw RuntimeError(call->m_parenthesis, "Expected to be callable");
    }

    if(arguments.size() != functionCallable->arity()) {
        std::ostringstream stream;
        stream << "Expected " << functionCallable->arity() << " arguments but " << arguments.size() << " present";
        throw RuntimeError(call->m_parenthesis, stream.str());
    }
    return functionCallable->call(this, arguments);
}

std::any Evaluator::visit(Function *func) {
    LoxCallable *function = new LoxFunction{func, m_environment, false};
    m_environment->define(func->m_name, function);
    return nullptr;
}

std::any Evaluator::visit(Logical *logical) {
    std::any left = evaluate(logical->m_left);

    if(logical->m_operator.m_token_type == TokenType::OR) {
        if(isTruthy(left)) return left;
    } else {
        if(!isTruthy(left)) return left;
    }

    return evaluate(logical->m_right);
}

bool Evaluator::isEqual(std::any left, std::any right) {
    if(left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
        return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
    } else if(left.type() == typeid(double) && right.type() == typeid(double)){
        return std::any_cast<double>(left) == std::any_cast<double>(right);
    } else if(left.type() == typeid(bool) && right.type() == typeid(bool)){
        return std::any_cast<bool>(left) == std::any_cast<bool>(right);
    }

    if(!left.has_value() && !right.has_value()) {
        return true;
    }

    return false;
}

bool Evaluator::isTruthy(std::any expr) {
    if(!expr.has_value()) return false;
    if(expr.type() == typeid(bool)) return std::any_cast<bool>(expr);
    return true;
}

std::any Evaluator::interpret(std::vector<Statement *> list) {
    try {
        for(Statement* stmt : list) {
            execute(stmt);
        }
        return 1;
    } catch (RuntimeError e) {
        runtimeError(e);
    }

    return 1;
}

void Evaluator::runtimeError(RuntimeError e) {
    m_has_runtime_error = true;
    m_error_reporter->error(e);
}

std::string Evaluator::stringify(std::any value) {
    if(!value.has_value()) return "null";

    if(value.type() == typeid(double) && is_int(std::any_cast<double>(value))) {
        return std::to_string((int) std::any_cast<double>(value));
    }

    if(value.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(value));
    }

    if(value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(value);
    }

    if(value.type() == typeid(bool)) {
        return std::any_cast<bool>(value) ? "true" : "false";
    }

    return "null";

}


std::any Evaluator::evaluate(Expression* expr) {
    return expr->accept(this);
}

std::any Evaluator::execute(Statement *stmt) {
    return stmt->accept(this);
}

std::any Evaluator::executeBlock(Block *block, Environment *environment) {
    Environment* prev = m_environment;
    try {
        m_environment = environment;
        for(Statement *stmt : *block->m_statements) {
            execute(stmt);
        }
    }
    catch(RuntimeError e) {
        m_environment = prev;
        runtimeError(e);
        return nullptr;
    }

    m_environment = prev;


    return nullptr;
}