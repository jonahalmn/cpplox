#ifndef VISITOR_H
#define VISITOR_H
// this file has been auto generated
#include "./type_defs.h"
template<class T> class Visitor {
public:
virtual T visit(Binary<T> *binary) = 0;
virtual T visit(Grouping<T> *grouping) = 0;
virtual T visit(Literal<T> *literal) = 0;
virtual T visit(Unary<T> *unary) = 0;
virtual ~Visitor(){};
};
#endif
