#include "loxClass.h"
#include "loxInstance.h"

std::any LoxClass::call(Evaluator *evaluator, std::vector<std::any> args) {
    LoxInstance *instance = new LoxInstance{this};
    return instance;
};

unsigned int LoxClass::arity() {
    return 0;
}

LoxCallable *LoxClass::findMethod(std::string name) {
    if(m_methods.find(name) != m_methods.end())
        return m_methods.at(name);

    return nullptr;
}
