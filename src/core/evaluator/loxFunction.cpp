#include "loxFunction.h"
#include "./evaluator.h"

std::any LoxFunction::call(Evaluator *evaluator, std::vector<std::any> arguments) {
    Environment *env = new Environment{m_closure};
    Environment *prev = evaluator->m_environment;
    for (unsigned int i = 0; i < m_declaration->m_params.size() ; i++)
    {
        env->define(m_declaration->m_params[i], arguments[i]);
    }

    try {
        evaluator->executeBlock(m_declaration->m_body, new Environment{env});
    } catch (ReturnExeption *e) {
        // restore to previous env before block
        evaluator->m_environment = prev;
        return e->m_value;
    }

    // 'this' is defined one scope above beacause of issue mentionned above.
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