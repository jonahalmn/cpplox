#include <iostream>
#include <fstream>
#include "./src/interpreter.h"
#include "./src/file.h"

int main(int argc, char* argv[]) {
    if ( argc <= 1 ) {
        std::cout << "Usage: " << argv[0] << " <FILENAME>" << std::endl;
        return 1;
    }

    std::cout << "Lox interpreter with script: " << argv[1] << std::endl;

    File file{ argv[1] };
    Interpreter interpreter{ &file };

    return 0;
}