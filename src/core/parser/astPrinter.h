#include <iostream>
#include <string>
#include <vector>
#include "./visitor.h"
#include "./binary.h"
#include "./expression.h"
#include "./grouping.h"
#include "./literal.h"
#include "./unary.h"

class AstPrinter : public Visitor<std::string> {
    public:
        std::string print(Expression<std::string> *expr) {
            return expr->accept(this);
        }

        virtual std::string visit(Binary<std::string> *binary) {
            std::vector<Expression<std::string>*> expressions{ binary->m_left, binary->m_right};
            return parenthize(binary->m_operator.m_lexeme, expressions);
        }

        virtual std::string visit(Grouping<std::string> *grouping) {
            std::vector<Expression<std::string>*> expressions{grouping->m_expression};
            return parenthize("group", expressions);
        }

        virtual std::string visit(Literal<std::string> *literal) {
            return literal->m_value;
        }

        virtual std::string visit(Unary<std::string> *unary) {
            std::vector<Expression<std::string>*> expressions{ unary->m_right};
            return parenthize(unary->m_operator.m_lexeme, expressions);
        }

    protected:
        std::string parenthize(std::string name, std::vector<Expression<std::string>*> exps) {
            std::string final_string;

            final_string += "(" + name;

            for(auto expr : exps) {
                final_string += " ";
                final_string += expr->accept(this);
            }

            final_string += ")";

            return final_string;

        }
};