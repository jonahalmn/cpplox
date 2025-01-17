#include "./environment.h"

void Environment::define(Token token, std::any object) {
    m_list.insert(std::pair<std::string, std::any>{token.m_lexeme, object});
}

std::any Environment::get(Token token) {
    if(m_list.find(token.m_lexeme) != m_list.end()) {
        if(m_list.at(token.m_lexeme).type() == typeid(nullptr)) throw RuntimeError(token, "Variable defined but not initialized.");
        return m_list.at(token.m_lexeme);
    }

    if(m_enclosing != nullptr) return m_enclosing->get(token);

    throw RuntimeError(token, "Undefined variable : " + token.m_lexeme);
    return nullptr;
}


std::any Environment::get_at(int distance, std::string name) {
    return ancestor(distance)->m_list.at(name);
}

Environment *Environment::ancestor(int depth) {
    Environment *env = this;
    for (int i = 0; i < depth; i++)
    {
       env = env->m_enclosing;
    }

    return env;
}

std::any Environment::assign(Token token, std::any object) {
    if(m_list.find(token.m_lexeme) != m_list.end()) {
        m_list.at(token.m_lexeme) = object;
        return m_list.at(token.m_lexeme);
    }

    if(m_enclosing != nullptr) return m_enclosing->assign(token, object);

    throw RuntimeError(token, "Undefined variable : " + token.m_lexeme);
}

std::any Environment::assign_at(int depth, Token token, std::any object) {
    return ancestor(depth)->m_list[token.m_lexeme] = object;
}