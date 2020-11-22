#include <iostream>
#include <string>
#include <vector>
#include <any>
#include "./visitor.h"
#include "./binary.h"
#include "./expression.h"
#include "./grouping.h"
#include "./literal.h"
#include "./unary.h"
#include "./ternary.h"


class AstPrinter : public Visitor {
    public:
        std::string print(Expression *expr) {
            return std::any_cast<std::string>(expr->accept(this));
        }

        virtual std::any visit(Binary *binary) {
            std::vector<Expression *> expressions{ binary->m_left, binary->m_right};
            return parenthize(binary->m_operator.m_lexeme, expressions);
        }

        virtual std::any visit(Grouping *grouping) {
            std::vector<Expression *> expressions{grouping->m_expression};
            return parenthize("group", expressions);
        }

        virtual std::any visit(Literal *literal) {
            return literal->m_value;
        }

        virtual std::any visit(Unary *unary) {
            std::vector<Expression *> expressions{ unary->m_right};
            return parenthize(unary->m_operator.m_lexeme, expressions);
        }

        virtual std::any visit(Ternary *ternary) {
            std::vector<Expression *> expressions{ ternary->m_condition, ternary->m_consequence, ternary->m_alternative };
            return parenthize("?:", expressions);
        }

    protected:
        std::string parenthize(std::string name, std::vector<Expression*> exps) {
            std::string final_string;

            final_string += "(" + name;

            for(auto expr : exps) {
                final_string += " ";
                std::any value = expr->accept(this);
                if(value.type() == typeid(bool)) {
                    std::any_cast<bool>(value) ? final_string += "true" : final_string += "false";
                } else {
                    final_string += std::any_cast<std::string>(expr->accept(this));
                }
            }

            final_string += ")";

            return final_string;

        }
};