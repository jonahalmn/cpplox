#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

void generateAst(std::string, std::string, std::vector<std::string>);
void generateVisitor(std::string, std::string, std::vector<std::string>);

int main(int argc, char* argv[]) {
    if ( argc <= 1 ) {
        std::cout << "Usage: " << argv[0] << " <PATH>" << std::endl;
        return 1;
    };

    std::vector<std::string> types{
        "Binary   : Expression *m_left, Token m_operator, Expression *m_right",
        "Grouping : Expression *m_expression",
        "Literal  : std::string m_value, std::string m_type",
        "Unary    : Token m_operator, Expression *m_right"
    };
    const char* base = "Expression";

    generateAst(argv[1], base, types);
    generateVisitor(argv[1], base, types);

    return 0;
}

std::string& trim(std::string& str)
{
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

std::string ucase(std::string str) {
    std::transform(
            str.begin(),
            str.end(),
            str.begin(),
            [](char c){ return std::toupper(c); });
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

std::string replace_all(std::string str, char c = ',', std::string by = ";") {
    while (str.find(c) != std::string::npos)
    {
        unsigned int index = str.find(c);
        str.replace(index, 1, by);
        str.insert(index + 1, "\n");
    }

    return str;
}

void generateVisitor(std::string output_dir, std::string base_class, std::vector<std::string> types) {
    std::string filename{output_dir + "visitor.h"};
    std::cout << base_class << std::endl;
    std::ofstream outfile(filename);

    outfile << "#ifndef VISITOR_H" << std::endl;
    outfile << "#define VISITOR_H" << std::endl;
    outfile << "// this file has been auto generated" << std::endl;
    outfile << "#include \"./type_defs.h\"" << std::endl;
    outfile << "class Visitor {" << std::endl;
    outfile << "public:" << std::endl;
    for(std::string type : types){
        std::string classname = type.substr(0, type.find(":"));
        std::string lower_classname = classname;
        std::transform(
            lower_classname.begin(),
            lower_classname.end(),
            lower_classname.begin(),
            [](char c){ return std::tolower(c); });
        outfile << "virtual std::any visit(" << trim(classname) << " *" << trim(lower_classname) << ") = 0;" << std::endl;
    };
    outfile << "virtual ~Visitor(){};" << std::endl;
    outfile << "};" << std::endl;
    outfile << "#endif" << std::endl;

    std::string typefilename{output_dir + "type_defs.h"};
    std::ofstream typefile(typefilename);


    typefile << "#ifndef TYPES_H" << std::endl;
    typefile << "#define TYPES_H" << std::endl;
    typefile << "class Expression;" << std::endl;
    typefile << "class Visitor;" << std::endl;
    for(std::string type : types){
        std::string classname = type.substr(0, type.find(":"));
        typefile << "class " << trim(classname) << ";" << std::endl;
    }
    typefile << "#endif" << std::endl;


}

std::string hydrate_members(std::vector<std::string> members) {
    std::string final_string;
    for (std::string member : members) {
        final_string += member + "{" + member + "},";
    }

    return final_string.substr(0, final_string.length() - 1);
}

void generateAst(std::string output_dir, std::string base_class, std::vector<std::string> types) {
    std::cout << output_dir << base_class << std::endl;

    std::string lower_base_class = base_class;

    std::transform(
            lower_base_class.begin(),
            lower_base_class.end(),
            lower_base_class.begin(),
            [](char c){ return std::tolower(c); }
        );

    std::string filename{output_dir + trim(lower_base_class) + ".h"};
    std::ofstream outfile(filename);

    outfile << "#ifndef EXPRESSION_H" << std::endl;
    outfile << "#define EXPRESSION_H" << std::endl;
    outfile << "#include <iostream>" << std::endl;
    outfile << "#include <any>" << std::endl;
    outfile << "#include \"./visitor.h\"" << std::endl;
    outfile << "class Expression {" << std::endl;
    outfile << "public:" << std::endl;
    outfile << "virtual std::any accept(Visitor *visitor) = 0;" << std::endl;
    outfile << "virtual ~Expression(){};" << std::endl;
    outfile << "};" << std::endl;
    outfile << "#endif" << std::endl;


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

        // std::cout << replace_all(arguments) << std::endl;

        std::vector<std::string> members{};
        std::string arguments_to_find = arguments;
        while (arguments_to_find.find("m_") != std::string::npos)
        {
            // consume first string part
            unsigned int found = arguments_to_find.find("m_");
            arguments_to_find = arguments_to_find.substr(found, arguments_to_find.length() - found);

            // isolate
            found = arguments_to_find.find("m_");
            unsigned int end_found = arguments_to_find.find(",");
            if(!end_found || end_found > arguments_to_find.length()) end_found = arguments_to_find.length();

            // std::cout << "found: " << found << ", " << end_found << std::endl;
            std::cout << arguments_to_find.substr(found, end_found) << std::endl;
            members.push_back(arguments_to_find.substr(found, end_found));
            // std::cout << arguments_to_find << std::endl;
            // std::cout << "[" << end_found << "," << arguments_to_find.length() << "]" << std::endl;

            arguments_to_find = arguments_to_find.substr(end_found, arguments_to_find.length() - end_found);

        }

        std::cout << "////ok////" << std::endl;
        

        outfile << "// this file has been auto generated" << std::endl;
        outfile << "#ifndef " << ucase(trim(classname)) << "_H" << std::endl;
        outfile << "#define " << ucase(trim(classname)) << "_H" << std::endl;
        outfile << "#include <iostream>" << std::endl;
        outfile << "#include <any>" << std::endl;
        outfile << "#include \"./type_defs.h\"" << std::endl;
        outfile << "#include \"./expression.h\"" << std::endl;
        outfile << "#include \"../lexer/token.h\"" << std::endl;
        outfile << "class " << trim(classname) << " : public " << base_class << " {" << std::endl;
        outfile << "public:" << std::endl;
        outfile << replace_all(arguments) << ";" << std::endl;
        outfile << trim(classname) << "(" << arguments << ") : " << hydrate_members(members) << "{}" << std::endl;
        outfile << "virtual std::any accept(Visitor *visitor){" << std::endl;
        outfile << "return visitor->visit(this);" << std::endl;
        outfile << "};" << std::endl;
        outfile << "virtual ~" << trim(classname) << "(){};" << std::endl;
        outfile << "};" << std::endl;
        outfile << "#endif" << std::endl;
    }



}