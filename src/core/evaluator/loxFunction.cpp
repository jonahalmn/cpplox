#include "loxFunction.h"
#include "./evaluator.h"

std::any LoxFunction::call(Evaluator *evaluator, std::vector<std::any> arguments) {
    Environment *env = new Environment{m_closure};
    for (unsigned int i = 0; i < m_declaration->m_params.size() ; i++)
    {
        env->define(m_declaration->m_params[i], arguments[i]);
    }

    try {
        evaluator->executeBlock(m_declaration->m_body, new Environment{env});
        // ancestor(1), because function declare two scopes
        // one for the function itself, and one for the block body
        // since it's not desirable, it's ok for demontration purpose.
        evaluator->m_environment = evaluator->m_environment->ancestor(1);
    } catch (ReturnExeption *e) {
        evaluator->m_environment = evaluator->m_environment->ancestor(1);
        return e->m_value;
    }

    if(m_is_initializer) return m_closure->get_at(0, "this");
    return nullptr;
}

unsigned int LoxFunction::arity() {
    return m_declaration->m_params.size();
}

LoxFunction *LoxFunction::bind(LoxInstance *instance) {
    Environment *env = new Environment{m_closure};
    env->define(Token{TokenType::THIS, "this", instance->m_klass->m_name.m_line}, instance);
    return new LoxFunction{m_declaration, env, false};
}