#include <iostream>
#include <fstream>
#include "./src/core/interpreter.h"
#include "./src/fs/file.h"
#include "./src/core/parser/reverseNotationner.h"
#include "./src/core/parser/binary.h"
#include "./src/core/parser/grouping.h"
#include "./src/core/parser/literal.h"
#include "./src/core/parser/unary.h"

#include "./src/core/lexer/token.h"

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