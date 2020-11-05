#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <iostream>
#include "./visitor.h"
template<class T> class Expression {
public:
virtual T accept(Visitor<T> *visitor) = 0;
virtual ~Expression(){};
};
#endif
