#include "loxFunction.h"
#include "./evaluator.h"

std::any LoxFunction::call(Evaluator *evaluator, std::vector<std::any> arguments) {
    Environment *env = new Environment{evaluator->m_global};
    for (unsigned int i = 0; i < m_declaration->m_params.size() ; i++)
    {
        env->define(m_declaration->m_params[i], arguments[i]);
    }

    evaluator->executeBlock(m_declaration->m_body, env);
    return nullptr;
}

unsigned int LoxFunction::arity() {
    return m_declaration->m_params.size();
}