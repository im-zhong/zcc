#include "ast/expression.h"
#include "core/grammar_abstract.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace cml;
_CML_BEGIN_

Token ExpressionParser::token = {};
int ExpressionParser::token_index = 0;
std::vector<Token> ExpressionParser::token_vector = {};
bool ExpressionParser::is_unary_expression_flag = true;
// 装填token vector
void ExpressionParser::init() {
    auto id = make_token(TERMINAL::ID, "i");
    auto add = make_token(TERMINAL::OP_ADD, "+");
    auto mul = make_token(TERMINAL::OP_MUL, "*");

    // i + i * i
    token_vector.push_back(id);
    token_vector.push_back(add);
    token_vector.push_back(id);
    token_vector.push_back(add);
    token_vector.push_back(id);
    token_vector.push_back(mul);
    token_vector.push_back(id);
    token_vector.push_back(add);
    token_vector.push_back(id);
}

Token ExpressionParser::next_token() {

    if (token_index < token_vector.size()) {
        ++token_index;
        return token_vector[token_index - 1];
    } else {
        return make_token(-1, "#");
    }
}

// expression
/*
expression :
        assignment-expression { ',' assignment-expression }
*/
expression_tree Expression::operator()() {
    // 这里返回的是一个森林啊
    // 并不是一颗树
    // 不行 他必须构建为一颗树
    // 只要用逗号把树分开就行了
    cout << "expression" << endl;
    auto tree = assignment_expression();
    // 先写一个树的后序遍历
    // 只要可以打印出求值顺序进行了
    // 这样我可以检查最后实现的对不对
    while (token.lexeme == ",") {
        auto comma_token = token;
        token = next_token();
        auto comma_tree = make_binary_tree_node(comma_token.lexeme);
        comma_tree->insert_right(tree);
        comma_tree->insert_left(assignment_expression());
        tree = comma_tree;
    }
    return tree;
}

/*
assignment-expression :
        conditional-expression
        unary-expression assign-operator assignment-expression
*/
expression_tree AssignmentExpression::operator()() {
    //// 要判断 if (token == D(conditional_expression))
    // conditional_expression(); // ? 是不是不太对啊
    //// 要判断 If (token == D(unary_expression...))
    // unary_expression();
    //// 接下来的这个token 应该是 assign_operator 中的一个
    //// assign_operator
    // token = make_token();
    // assignment_expression();
    //  error
    //  return

    // 下面这些是书上的实现
    // 说实话这个实现我是有点看不懂的
    // 3.26 看懂了
    // condition expression 分析正确了 那么就分析正确了
    // 如果分析错误了 就相当域分析了一个unary expression
    // 那么接下来就是
    auto tree = conditional_expression();
    // 如果这确实是一个condition-expression 那么到这里就应该分析完了
    if (is_unary_expression_flag && is_assign_operator(token)) {
        // 但是如果这里只是相当于分析了一个unary expression
        // 那么我们就需要分析一个 unary = assignment 这样的句子
        // 所以tree就作为了左子树
        auto assign_token = token; // 这个token保存赋值符号
        token = next_token();
        // 我需要创建一颗树
        // 这棵树以 uanry为左子树
        // token符号为根
        // 下一个assignment为右子树
        // 好麻烦啊 我或许应该可以允许直接插入一棵树
        auto assign_tree = make_binary_tree_node(assign_token.lexeme);
        assign_tree->insert_left(tree);
        assign_tree->insert_right(assignment_expression());
        tree = assign_tree;
    }
    return tree;
}

/*
conditional-expression :
        binary-expression [ '?' expression ':' conditional-expression ]
*/
expression_tree ConditionalExpression::operator()() {
    is_unary_expression_flag = true;
    auto tree = binary_expression();
    if (token.lexeme == "?") {
        is_unary_expression_flag = false;
        auto question_token = token;
        token = next_token();
        auto question_tree = make_binary_tree_node(question_token.lexeme);
        question_tree->insert_left(tree);
        tree = expression();
        // node->left = expression(); // 书中使用了 expression(':')
        // 其中冒号作为输入出错时的停止符号
        if (token.lexeme == ":") {
            auto colon_token = token;
            token = next_token();
            auto colon_tree = make_binary_tree_node(colon_token.lexeme);
            question_tree->insert_right(colon_tree);
            colon_tree->insert_left(tree);
            colon_tree->insert_right(conditional_expression());
            tree = question_tree;
            // node->right = conditional_expression();
        } else {
            // error 问号表达式没有写全
            cerr << "问号表达式书写错误 漏掉了 分号" << endl;
        }
    }
    return tree;
}

expression_tree BinaryExpression::operator()() {
    // 假设我有一张表
    // 通过传递token
    // 我就可以获得token对应的符号的运算符优先级
    // 那么我根据优先级就可以写出递归关系
    // auto node = unary_expression();
    // while (is_binary_operator(token))
    //{
    //	token = next_token();
    //	node = make_tree(0, nullptr, node, unary_expression()); //
    // 这样写无法体现优先级 结合性等
    //}
    // return node;
    return parser(4);
}

/*
binary-expression :
        unary-expression { binary-operator unary-expression }
*/
expression_tree BinaryExpression::parser(int priority) {
    int current_priority = 0;
    auto tree = unary_expression();
    for (current_priority = get_priority(token); current_priority >= priority;
         --current_priority) {
        while (get_priority(token) == current_priority) {
            is_unary_expression_flag = false;
            auto operator_token = token;
            token = next_token();
            auto operator_tree = make_binary_tree_node(operator_token.lexeme);
            operator_tree->insert_left(tree);
            operator_tree->insert_right(parser(current_priority + 1));
            tree = operator_tree;
            // node = make_tree(op, nullptr, node, parser(current_priority +
            // 1));
        }
    }
    return tree;
}

/*
unary-expression :
        unary-operator unary-expression		first(unary-operator): ++ -- & *
+ - ~ ! postfix-expression
first(postfix-expression): first(primary-expression): identifier constant
string-lietral '('
        '(' type-name ')' unary-expression	first : '('
        "sizeof" unary-expression			first : sizeof
        "sizeof" '(' type-name ')'			first : sizeof

        我们可以看到这里仍然发生了first冲突
        但是不急
        我们仍然可以使用在condition-expression那里的技巧
        分析一下 并设置一个标志位就可以了
*/
expression_tree UnaryExpression::operator()() {
    expression_tree tree = nullptr;

    // if t == D(postfix expression)
    if (is_unary_operator(token)) {
        auto unary_token = token;
        token = next_token();
        auto unary_tree = make_binary_tree_node(unary_token.lexeme);
        unary_tree->insert_left(unary_expression());
        tree = unary_tree;
    } else if (token.lexeme == "sizeof") // == sizeof
    {
        tree = sizeof_expression();
    } else if (token.category == static_cast<uint8_t>(TERMINAL::ID) ||
               is_literal(token)) {
        // 判断是否为 id constant string-lietral
        tree = postfix_expression(false);
    } else if (token.lexeme == "(") {
        // postfix - expression					first(postfix -
        // expression) : first(primary - expression) : identifier constant
        // string - lietral '('
        // 	'(' type - name ')' unary - expression	first : '('
        token = next_token();
        // 在调用postfix operator 的时候 我们需要让他直到
        // 我们是不是把他的'('给吃掉了
        // 设置一个参数就可以了
        if (is_type_name(token)) // == type
        {
            // 这里是一个类型转换
            auto type_token = token;
            // 创建一颗类型转换树
            auto type_cast_tree = make_binary_tree_node(type_token.lexeme);
            token = next_token();
            if (token.lexeme == ")") {
                token = next_token();
            } else {
                // error
                cerr << "缺少）" << endl;
            }
            type_cast_tree->insert_right(unary_expression());
            tree = type_cast_tree;
        } else // 就有可能是expression 但是要交给下一个函数来调用
        {
            tree = postfix_expression(true);
        }
    }

    return tree;
}

bool SizeofExpression::is_unary_expression(Token token) {
    return unary_expression.is_unary_operator(token) ||
           token.lexeme == "sizeof" || token.category == TERMINAL::ID ||
           unary_expression.is_literal(token);
}
/*
        "sizeof" unary-expression
        "sizeof" '(' type-name ')'
*/
expression_tree SizeofExpression::operator()() {
    expression_tree tree = nullptr;
    if (token.lexeme == "sizeof") {
        auto sizeof_token = token;
        // 创建一颗sizeof树
        auto sizeof_tree = make_binary_tree_node(sizeof_token.lexeme);
        // 这里仍然存在需要提前读取一个圆括号的问题
        token = next_token();
        if (is_unary_expression(token)) {
            // 这里考虑的太复杂了
            // if (token.lexeme == "(")
            //{
            //	// 那么这里可能是 (type name)
            //	// 也可能是 postfix-expression
            //	// 所以在这里我们还需要向前看一眼
            //	token = next_token();
            //	if (unary_expression.is_type_name(token))
            //	{
            //		// 获得typename
            //		// 因为还没有实现 所以暂时使用int代替
            //		sizeof_tree->insert_right(string("int"));
            //		tree = sizeof_tree;
            //	}
            //	else
            //	{
            //		// 如果是其他的 那么我们需要通知 unary-expression
            //		// 我们吃掉了
            //	}

            //}
            // else
            //{
            //	sizeof_tree->insert_right(unary_expression());
            //	tree = sizeof_tree;
            //}
            sizeof_tree->insert_right(unary_expression());
            tree = sizeof_tree;
        } else {
            cerr << "sizeof 后面需要一个unary expression" << endl;
        }
    } else {
        cerr << "sizeof忘了写吧" << endl;
    }
    return tree;
}

/*
postfix-expression :
        primary-expression { postfix-operator }

postfix-operator :
        '[' expression ']'
        '(' [ assignment-expression { ',' assignment-expression } ] ')'
        '.' identifier
        "->" identifier
        "++"
        "--"
*/
expression_tree PostfixExpression::operator()(bool is_round_bracket) {
    auto tree = primary_expression();

    // 使用while循环避免递归调用自身
    while (is_postfix_operator(token)) {
        auto postfix_token = token;
        // 使用这个token创建一颗树
        auto postfix_tree = postfix_operator();
        postfix_tree->insert_left(tree);
        tree = postfix_tree;
    }

    // while (is_postfix_operator(token)) // 在这里这么判断太草率了
    // 这些后缀表达式比较复杂
    //{

    //}
    // for (;;)
    //{
    //	// 数组下标
    //	if (token.lexeme == "[")
    //	{
    //		token = next_token();
    //		auto expression_node = expression();
    //		if (token.lexeme == "]")
    //		{
    //			// right
    //		}
    //		else
    //		{
    //			// error
    //		}
    //	}
    //	// 函数参数列表
    //	else if (token.lexeme == "(")
    //	{
    //		token = next_token();
    //		// 不断寻找直到下一个 )
    //	}
    //	// 结构体访问
    //	else if (token.lexeme == ".")
    //	{
    //		token = next_token();
    //		if (token.category == TERMINAL::ID)
    //		{
    //			// right
    //		}
    //		else
    //		{
    //			// error
    //		}
    //	}
    //	// 结构体指针
    //	else if (token.lexeme == "->")
    //	{
    //		token = next_token();
    //		if (token.category == TERMINAL::ID)
    //		{
    //			// right
    //		}
    //		else
    //		{
    //			// error
    //		}
    //	}
    //	// 自增
    //	else if (token.lexeme == "++")
    //	{

    //	}
    //	else if (token.lexeme == "--")
    //	{

    //	}
    //	else
    //	{
    //		// error
    //	}
    //}
    return tree;
}

/*
postfix-operator :
        '[' expression ']'
        '(' [ assignment-expression { ',' assignment-expression } ] ')'
        '.' identifier
        "->" identifier
        "++"
        "--"
*/
expression_tree PostfixOperator::operator()() {
    expression_tree postfix_tree = nullptr;
    if (token.lexeme == "[") {
        token = next_token();
        // 这是一个数组后缀
        // 构建一颗array树
        auto array_tree = make_binary_tree_node(std::string("[]"));
        // 他的右子树是数组下标
        array_tree->insert_right(expression());

        if (token.lexeme == "]") {
            token = next_token();
        } else {
            cerr << "数组下标缺少]" << endl;
        }
        return array_tree;
    } else if (token.lexeme == "(") {
        // 这波是函数调用
        // 创建一颗函数调用树
        auto function_tree = make_binary_tree_node(string("()"));
        // 我感觉每一个分支写成一个函数更好
        // 这样才能体现我把每一个符号写成类的意义
        token = next_token();
        if (unary_expression.is_unary_operator(token) ||
            unary_expression.is_literal(token) ||
            token.category == TERMINAL::ID || token.lexeme == "(" ||
            token.lexeme == "sizeof") {
            // 这里面的其实就是参数 每一个参数都是一个右子树
            // 创建一颗参数树
            auto parameter_tree = assignment_expression();
            while (token.lexeme == ",") {
                auto comma_token = token;
                token = next_token();
                auto comma_tree = make_binary_tree_node(comma_token.lexeme);
                comma_tree->insert_right(parameter_tree);
                comma_tree->insert_left(assignment_expression());
                parameter_tree = comma_tree;
            }
            // 参数列表读取完毕

            function_tree->insert_right(parameter_tree);
        }

        // 检查括号师傅完整
        if (token.lexeme == ")") {
            token = next_token();
        } else {
            cerr << "函数调用缺少)" << endl;
        }

        return function_tree;

    } else if (token.lexeme == ".") {
        // 这波是结构体成员
        auto dot_token = token;
        token = next_token();
        // 构建一颗结构体成员树
        auto dot_tree = make_binary_tree_node(dot_token.lexeme);
        if (token.category == TERMINAL::ID) {
            dot_tree->insert_right(token.lexeme);
            token = next_token();
        } else {
            cerr << ".后面应该跟一个identfier" << endl;
        }
        return dot_tree;
    } else if (token.lexeme == "->") {
        auto dot_token = token;
        token = next_token();
        // 构建一颗结构体成员树
        auto dot_tree = make_binary_tree_node(dot_token.lexeme);
        if (token.category == TERMINAL::ID) {
            dot_tree->insert_right(token.lexeme);
            token = next_token();
        } else {
            cerr << ".后面应该跟一个identfier" << endl;
        }
        return dot_tree;
    } else if (token.lexeme == "++") {
        auto inc_token = token;
        auto inc_tree = make_binary_tree_node(inc_token.lexeme);
        token = next_token();
        return inc_tree;
    } else if (token.lexeme == "--") {
        auto dec_token = token;
        auto dec_tree = make_binary_tree_node(dec_token.lexeme);
        token = next_token();
        return dec_tree;
    } else {
        cerr << "表达式错误" << endl;
    }
    return postfix_tree;
}

// primary expression
/*
primary-expression :
        identifier
        constant             这个应该就是constant expression吧
        string-literal
        '(' expression ')'
*/
expression_tree PrimaryExpression::operator()() {
    std::cout << "primary expression" << std::endl;
    expression_tree tree = nullptr;
    switch (token.category) // 其实常量有很多种的 后缀可以表示很多 int long
                            // unsigned int unsigned long
    {
    case TERMINAL::LIT_INT:
        std::cout << "literal int" << endl;
    case TERMINAL::LIT_REAL: // float double
        std::cout << "literal real";
    case TERMINAL::LIT_STR: // 这个也有两种把 char const char*
        std::cout << "literal string";
    case TERMINAL::ID:
        std::cout << "identifier" << endl;
    case TERMINAL::LIT_ENUM:
        std::cout << "enum" << std::endl;
    case TERMINAL::LIT_CHAR:
        std::cout << "char" << endl;
        tree = make_binary_tree_node(token.lexeme);
        token = next_token();
        break;
    case TERMINAL::DLM_SOE:

        tree = expression();
        if (token.category == TERMINAL::DLM_EOE) {
            token = next_token();
        } else {
            cerr << "缺少)" << endl;
        }
    }
    // token = next_token();
    // return make_tree(token.category, nullptr, nullptr, nullptr);

    std::cout << "primary expression" << endl;
    return tree;
}

_CML_END_
