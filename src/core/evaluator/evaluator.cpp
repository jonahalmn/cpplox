#include "./evaluator.h"

std::any Evaluator::visit(Literal *literal) {
    return literal->m_value;
}

std::any Evaluator::visit(Grouping *grouping) {
    return evaluate(grouping->m_expression);
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
        throw RuntimeError(binary->m_operator, "couldnt understand operator");
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