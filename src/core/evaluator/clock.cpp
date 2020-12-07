#include "clock.h"

unsigned int Clock::arity() {
    return 0;
}

std::any Clock::call(Evaluator *evaluator, std::vector<std::any> arguments) {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);

    return (double) value.count();
}