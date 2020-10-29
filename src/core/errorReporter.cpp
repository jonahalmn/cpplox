#include "./errorReporter.h"

ErrorReporter* ErrorReporter::m_instance = nullptr;

ErrorReporter* ErrorReporter::getInstance() {
    if(m_instance == nullptr) {
        m_instance = new ErrorReporter;
    }

    return m_instance;

}

int ErrorReporter::had_error() {
    return m_had_error;
}

void ErrorReporter::error(unsigned int line, const char* message) {
    report(line, "", message);
}

void ErrorReporter::report(unsigned int line, const char* where, const char* message) {
    std::cerr << "[line " << line << "] Error "
        << where << ":" << message << std::endl;
    m_had_error = 1;
}