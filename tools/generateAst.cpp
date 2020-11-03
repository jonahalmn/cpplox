#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

void generateAst(std::string, std::string, std::vector<std::string>);

int main(int argc, char* argv[]) {
    if ( argc <= 1 ) {
        std::cout << "Usage: " << argv[0] << " <PATH>" << std::endl;
        return 1;
    };

    std::vector<std::string> types{
        "Binary   : Expression m_left, Token m_operator, Expression m_right",
        "Grouping : Expression m_expression",
        "Literal  : Object m_value",
        "Unary    : Token m_operator, Expr m_right"
    };
    const char* base = "Expression";

    generateAst(argv[1], base, types);

    return 0;
}

std::string& trim(std::string& str)
{
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

std::string trim_string(std::string &str) {
    unsigned int n = str.length();

    unsigned int iterator = 0;
    unsigned int to_erase_before = 0;
    unsigned int to_erase_after = 0;

    while (str[iterator] == ' ')
    {
        iterator++;
        to_erase_before++;
    }

    iterator = n;
    while (str[iterator] == ' ') {
        iterator--;
        to_erase_after++;
    }

    std::string result = str.substr(to_erase_before, n - to_erase_before - to_erase_after);

    return result;
}

std::string replace_all(std::string str, char c = ',') {
    while (str.find(c) != std::string::npos)
    {
        unsigned int index = str.find(c);
        str.replace(index, 1, ";");
        str.insert(index + 1, "\n");
    }

    return str;
}

void generateAst(std::string output_dir, std::string base_class, std::vector<std::string> types) {
    std::cout << output_dir << base_class << std::endl;
    for (std::string type : types)
    {
        std::string classname = type.substr(0, type.find(":"));
        std::string lower_classname = classname;
        std::transform(
            lower_classname.begin(),
            lower_classname.end(),
            lower_classname.begin(),
            [](char c){ return std::tolower(c); }
        );

        std::string arguments = type.substr(type.find(":") + 1, type.length() - type.find(':'));
        trim_string( arguments );

        std::string filename{output_dir + trim(lower_classname) + ".h"};

        std::ofstream outfile(filename);

        std::cout << replace_all(arguments) << std::endl;

        outfile << "#include <iostream>" << std::endl;
        outfile << "#include \"./expression.h\"" << std::endl;
        outfile << "#include \"../lexer/token.h\"" << std::endl;
        outfile << "class " << trim(classname) << " : " << base_class << "{" << std::endl;
        outfile << replace_all(arguments) << ";" << std::endl;
        outfile << "};" << std::endl;
    }

}