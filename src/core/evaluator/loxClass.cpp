#include "loxClass.h"
#include "loxInstance.h"
#include "loxFunction.h"

std::any LoxClass::call(Evaluator *evaluator, std::vector<std::any> args) {
    LoxInstance *instance = new LoxInstance{this};
    LoxCallable *initializer = findMethod("init");
    if(initializer) {
        ((LoxFunction *) initializer)->bind(instance)->call(evaluator, args);
    }

    return instance;
};

unsigned int LoxClass::arity() {
    LoxCallable *initializer = findMethod("init");
    if (!initializer) return 0;
    unsigned int arity = ((LoxFunction *) initializer)->arity();
    return arity;
}

LoxCallable *LoxClass::findMethod(std::string name) {
    if(m_methods.find(name) != m_methods.end())
        return m_methods.at(name);

    return nullptr;
}

LoxCallable *LoxClass::findStatic(std::string name) {
    if(m_statics.find(name) != m_statics.end())
        return m_statics.at(name);

    throw NonStaticError();
    return nullptr;
}

std::any LoxClass::get(Token name) {
    return this->findStatic(name.m_lexeme);
}

// std::any LoxClass::set(Token name, std::any value) {
//     return nullptr;
// }
