#include "types.h"
#include "loxCallable.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <any>

#ifndef CLOCK_H
#define CLOCK_H

class Clock : public LoxCallable {
    public:
        Clock() {};
        virtual unsigned int arity();
        virtual std::any call(Evaluator *evaluator, std::vector<std::any> arguments);
        virtual ~Clock() {};
};

#endif