#include <iostream>
#include <vector>
#include <any>
#include "../visitor.h"
#include "./expression.h"
#include "./binary.h"
#include "./grouping.h"
#include "./literal.h"
#include "./unary.h"

class ReverseNotationner: Visitor {
    public:
        std::string print(Expression *expr) {
            return std::any_cast<std::string>(expr->accept(this));
        }

        std::any visit(Binary *binary) {
            std::vector<Expression *> expressions{binary->m_left, binary->m_right};
            return notation(binary->m_operator.m_lexeme, expressions);
        }

        std::any visit(Unary *unary) {
            std::vector<Expression *> expressions{unary->m_right};
            return notation(unary->m_operator.m_lexeme, expressions);
        }

        std::any visit(Grouping *group) {
            std::vector<Expression *> expressions{group->m_expression};
            return notation("", expressions);
        }

        std::any visit(Literal *literal) {
            return literal->m_value;
        }

    private:
        std::string notation(std::string exp_operator, std::vector<Expression *> exprs) {
            std::string final_string;


            for(auto expr : exprs) {
                final_string += std::any_cast<std::string>(expr->accept(this));
                final_string += " ";
            }
            final_string = final_string + exp_operator;

            return final_string;
        }

};