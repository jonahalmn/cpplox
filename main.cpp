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
    ReverseNotationner printer;

    Token token_m{TokenType::MINUS, "-", 0};
    std::string val{"123"};
    Literal literal{val};
    Unary unary1{token_m, &literal};
    Unary unary2;
    Binary binary;
    Token token{TokenType::STAR, "*", 0};
    unary1.m_right = &literal;
    unary1.m_operator = token_m;

    unary2.m_right = &literal;
    unary2.m_operator = token_m;
    binary.m_operator = token;
    binary.m_left = &unary1;
    binary.m_right = &unary2;

    std::cout << printer.print(&binary) << std::endl;

    return 0;
}