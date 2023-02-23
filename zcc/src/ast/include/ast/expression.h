#pragma once

#include "core/grammar_abstract.h"
#include "scan/token.h"
#include "tree.h"
#include <map>
#include <set>
#include <string>

_CML_BEGIN_

using expression_tree = std::shared_ptr<cml::binary_tree_node<std::string>>;

class ExpressionParser {
  public:
    static std::vector<Token> token_vector;
    static Token token;
    static int token_index;
    static void init();
    Token next_token();
    // 用来判断是否位condition expression
    static bool is_unary_expression_flag;
};

class Expression : public ExpressionParser {
  public:
    expression_tree operator()();
};

class AssignmentExpression : public ExpressionParser {
  public:
    AssignmentExpression() {
        assign_operatos.insert("=");
        assign_operatos.insert("+=");
        assign_operatos.insert("-=");
        assign_operatos.insert("*=");
        assign_operatos.insert("/=");
        assign_operatos.insert("%=");
        assign_operatos.insert("<<=");
        assign_operatos.insert(">>=");
        assign_operatos.insert("&=");
        assign_operatos.insert("^=");
        assign_operatos.insert("|=");
    }

    expression_tree operator()();

    bool is_assign_operator(Token token) {
        return assign_operatos.find(token.lexeme) != assign_operatos.end();
    }

    // 其实就是uanry_expression
    bool is_assignment_expression(Token token) { return false; }

    std::set<std::string> assign_operatos;
};

class BinaryExpression : public ExpressionParser {
  public:
    BinaryExpression() {
        operator_priority.insert({",", 1});

        operator_priority.insert({"=", 2});
        operator_priority.insert({"+=", 2});
        operator_priority.insert({"-=", 2});
        operator_priority.insert({"*=", 2});
        operator_priority.insert({"/=", 2});
        operator_priority.insert({"%=", 2});
        operator_priority.insert({"&=", 2});
        operator_priority.insert({"^=", 2});
        operator_priority.insert({"|=", 2});
        operator_priority.insert({"<<=", 2});
        operator_priority.insert({">>=", 2});

        operator_priority.insert({"?:", 3});

        operator_priority.insert({"||", 4});

        operator_priority.insert({"&&", 5});

        operator_priority.insert({"|", 6});

        operator_priority.insert({"^", 7});

        operator_priority.insert({"&", 8});

        operator_priority.insert({"==", 9});
        operator_priority.insert({"!=", 9});

        operator_priority.insert({"<", 10});
        operator_priority.insert({">", 10});
        operator_priority.insert({"<=", 10});
        operator_priority.insert({">=", 10});

        operator_priority.insert({"<<", 11});
        operator_priority.insert({">>", 11});

        operator_priority.insert({"+", 12});
        operator_priority.insert({"-", 12});

        operator_priority.insert({"*", 13});
        operator_priority.insert({"/", 13});
        operator_priority.insert({"%", 13});

        // 其余的操作符的优先级均大于13
    }

    expression_tree operator()();

    bool is_binary_operator(Token token) const {
        auto priority = get_priority(token);
        return priority <= 13 && priority >= 4;
    }

    int get_priority(Token token) const {
        auto r = operator_priority.find(token.lexeme);
        if (r != operator_priority.end()) {
            return r->second;
        } else {
            return 0;
        }
    }

    expression_tree parser(int priority);

    // 首先制作一张简单的
    std::map<std::string, int> operator_priority;
};

class ConditionalExpression : public ExpressionParser {
  public:
    expression_tree operator()();
};

class PostfixExpression : public ExpressionParser {
  public:
    PostfixExpression() {
        postfix_operators.insert("[");
        postfix_operators.insert("(");
        postfix_operators.insert(".");
        postfix_operators.insert("->");
        postfix_operators.insert("++");
        postfix_operators.insert("--");
    }

    expression_tree operator()(bool is_round_bracket);

    bool is_postfix_operator(Token token) {
        return postfix_operators.find(token.lexeme) != postfix_operators.end();
    }

    std::set<std::string> postfix_operators;
};

class PrimaryExpression : public ExpressionParser {
  public:
    expression_tree operator()();
};

class UnaryExpression : public ExpressionParser {
  public:
    UnaryExpression() {
        unary_operator.insert("++");
        unary_operator.insert("--");
        unary_operator.insert("&");
        unary_operator.insert("*");
        unary_operator.insert("+");
        unary_operator.insert("+");
        unary_operator.insert("~");
        unary_operator.insert("!");
    }

    bool is_literal(Token token) {
        return token.category == TERMINAL::LIT_CHAR ||
               token.category == TERMINAL::LIT_ENUM ||
               token.category == TERMINAL::LIT_INT ||
               token.category == TERMINAL::LIT_REAL ||
               token.category == TERMINAL::LIT_STR ||
               token.category == TERMINAL::ID;
    }

    bool is_type_name(Token token) { return false; }

    expression_tree operator()();

    bool is_unary_operator(Token token) {
        return unary_operator.find(token.lexeme) != unary_operator.end();
    }

    std::set<std::string> unary_operator;
};

class SizeofExpression : public ExpressionParser {
  public:
    expression_tree operator()();

    bool is_unary_expression(Token token);
};

class PostfixOperator : public ExpressionParser {
  public:
    expression_tree operator()();
};

static PrimaryExpression primary_expression;
static PostfixExpression postfix_expression;
static Expression expression;
static ConditionalExpression conditional_expression;
static BinaryExpression binary_expression;
static UnaryExpression unary_expression;
static AssignmentExpression assignment_expression;
static SizeofExpression sizeof_expression;
static PostfixOperator postfix_operator;

_CML_END_