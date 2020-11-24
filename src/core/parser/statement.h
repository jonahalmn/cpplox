#ifndef Statement_H
#define Statement_H
#include <iostream>
#include <any>
#include "../visitor.h"
class Statement {
public:
virtual std::any accept(Visitor *visitor) = 0;
virtual ~Statement(){};
virtual Statement *clone() = 0;
};
#endif
