#include <iostream>
#include <vector>
#include "./interpreter.h"
#include "./errorReporter.h"

void Interpreter::run() {

    std::cout << "running with program: " << m_string << std::endl;
    m_scanner.add_source(m_string);
    m_scanner.scan_tokens();
    if (m_error_reporter->had_error()) {
        std::cout << "error raised" << std::endl;
        std::exit(1);
    }
    Parser parser{m_scanner.get_tokens()};
    std::vector<Statement *> stmts = parser.parse();
    if(ErrorReporter::getInstance()->had_error()) {
        std::cout << "panic mode: parser encount error" << std::endl;
        exit(1);
    }
    
    Evaluator evaluator;
    std::any value = evaluator.interpret(stmts);
    if(ErrorReporter::getInstance()->had_runtime_error()) exit(70);
    // std::cout << evaluator.stringify(value) << std::endl;
}