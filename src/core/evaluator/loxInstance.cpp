#include "./loxInstance.h"
#include "./loxFunction.h"
#include "./loxClass.h"

std::any LoxInstance::get(Token name) {
    if(m_fields.find(name) != m_fields.end())
        return m_fields.at(name);

    LoxFunction *method = (LoxFunction *) m_klass->findMethod(name.m_lexeme);
    if(method) return (LoxCallable *) method->bind(this);

    throw RuntimeError{name, std::string{"undefined property " + name.m_lexeme}.c_str()};
}

std::any LoxInstance::set(Token name, std::any value) {
    m_fields[name] = value;
    return value;
}