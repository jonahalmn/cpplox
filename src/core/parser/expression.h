#ifndef Expression_H
#define Expression_H
#include <iostream>
#include <any>
#include "../visitor.h"
class Expression {
public:
virtual std::any accept(Visitor *visitor) = 0;
virtual ~Expression(){};
virtual Expression *clone() = 0;
};
#endif
