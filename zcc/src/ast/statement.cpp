
#include "ast/statement.h"
#include <cstddef>
#include <iostream>
#include <list>
#include <memory>
#include <vector>

using namespace std;
_CML_BEGIN_

int StatementParser::token_index = 0;
Token StatementParser::token = {};
std::set<std::string> StatementParser::statement_first =
    StatementParser::make_statement_first();
// 这里没有必要加move 因为返回值本身就是右值了
// 所需要做的是为左边的对象提供移动操作
std::vector<Token> StatementParser::token_vector =
    std::move(make_token_vector());

bool StatementParser::is_expression(Token token) { return token.lexeme == "e"; }

StatementStructure StatementParser::expression() {
    cout << "expression" << endl;
    token = next_token();

    // 2023/2/23: todo
    return std::make_shared<StatementStructureBase>();
}

Token StatementParser::next_token() {
    if (token_index < token_vector.size()) {
        ++token_index;
        return token_vector[token_index - 1];
    } else {
        return {static_cast<uint8_t>(-1), "#"};
    }
}

// 帮你调用next token 并且检查是不是你想要的token
bool StatementParser::next_token(Token token) { return next_token() == token; }

bool StatementParser::next_token(const std::string& lexeme) {
    auto temp = token;
    token = next_token();
    return temp.lexeme == lexeme;
}

bool StatementParser::next_token(uint8_t category) {
    return next_token().category == category;
}

bool StatementParser::is_statement(Token token) {
    return statement_first.find(token.lexeme) != statement_first.end();
}

std::set<std::string> StatementParser::make_statement_first() {
    std::set<std::string> first;
    first.insert("e"); // 特殊的表达式
    first.insert("{"); // compound expression
    first.insert(";");

    first.insert("continue");
    first.insert("break");
    first.insert("return");
    first.insert("while");
    first.insert("do");
    first.insert("for");
    first.insert("if");
    first.insert("swithc");
    first.insert("case");
    first.insert("default");

    return first;
}

std::vector<Token> StatementParser::make_token_vector() {
    std::vector<Token> temp;
    auto e = make_token(TERMINAL::ID, "e");
    auto e_begin = make_token(TERMINAL::DLM_SOE, "(");
    auto e_end = make_token(TERMINAL::DLM_EOE, ")");
    auto block_begin = make_token(TERMINAL::DLM_SOS, "{");
    auto block_end = make_token(TERMINAL::DLM_EOS, "}");
    auto semicolon = make_token(TERMINAL::DLM_SEMIC, ";");
    auto for_token =
        make_token(TERMINAL::RES_FOR, "for"); // 这样难道不会发生一次赋值吗
    auto if_token = make_token(TERMINAL::RES_IF, "if");
    auto else_token = make_token(TERMINAL::RES_ELSE, "else");

    temp.push_back(for_token);

    temp.push_back(e_begin);
    temp.push_back(e);
    temp.push_back(semicolon);
    temp.push_back(e);
    temp.push_back(semicolon);
    temp.push_back(e);
    temp.push_back(e_end);

    temp.push_back(block_begin);

    temp.push_back(if_token);
    temp.push_back(e_begin);
    temp.push_back(e);
    temp.push_back(e_end);

    temp.push_back(block_begin);
    temp.push_back(e);
    temp.push_back(semicolon);
    temp.push_back(block_end);

    temp.push_back(else_token);

    temp.push_back(block_begin);
    temp.push_back(e);
    temp.push_back(semicolon);
    temp.push_back(block_end);

    temp.push_back(block_end);

    return temp;
}

/*
statement :
        compound-statement
        expression-statement
        selection-statement
        iterator-statement
        // goto-statement 不进行实现
        labeled-statement
        jump-statement
        null-statement

null-statement :
        ';'

jump_statement :
        "continue" ';'
        "break" ';'
        "return" [ expression ] ';'

iteration-statement :
        "while" '(' expression ')' statement
        "do" statement "while" '(' expression ')' ';'
        "for" '(' [ expression ] ';' [ expression ] ';' [ expression ] ')'
statement

selection-statement :
        "if" '(' expression ')' statement
        "if" '(' expression ')' statement "else" statement
        "switch" '(' expression ')' statement

expression-statement :
        expression ';'

labeled-statement :
        "case" constant-expression ':' statement
        "default" ':' statement

compound-statement :
        '{' { declaration } { statement } '}'
*/

// 只不过这里需要模拟一个expression
// 然后就是不要有declaration 或者我也可以模拟一个

StatementStructure Statement::operator()() {
    cout << "statement" << endl;
    StatementStructure structure = nullptr;
    // 句子的语法分析是最简单的了 因为所有的句子的first集都不一样
    // 设计的时候就想到了这一点 所以实现起来很方便
    // expression
    // ; continue break return while do for if switch case defualt {
    if (token.lexeme == ";") {
        structure = null_statement();
    } else if (token.lexeme == "continue") {
        structure = continue_statement();
    } else if (token.lexeme == "break") {
        structure = break_statement();
    } else if (token.lexeme == "return") {
        structure = return_statement();
    } else if (token.lexeme == "while") {
        structure = while_statement();
    } else if (token.lexeme == "do") {
        structure = do_while_statement();
    } else if (token.lexeme == "for") {
        structure = for_statement();
    } else if (token.lexeme == "if") {
        structure = if_statement();
    } else if (token.lexeme == "switch") {
        structure = switch_statement();
    } else if (token.lexeme == "case") {
        structure = case_statement();
    } else if (token.lexeme == "default") {
        structure = default_statement();
    } else if (token.lexeme == "{") {
        structure = compound_statement();
    } else if (is_expression(token)) {
        structure = expression();
    }

    // 我需要在这里检查是否为真确的输入
    return structure;
}

/*
compound-statement :
        '{' { declaration } { statement } '}'
*/
StatementStructure CompoundStatement::operator()() {
    cout << "compound statement" << endl;
    token = next_token(); // read {
    // 没有declaration
    std::list<StatementStructure> compound_structures;
    while (is_statement(token)) {
        compound_structures.push_back(statement());
    }
    if (!next_token("}"))
        cerr << "缺少}" << endl;
    return make_compound(compound_structures);
}

/*
        "default" ':' statement
*/
StatementStructure DefaultStatement::operator()() {
    cout << "default" << endl;
    token = next_token(); // read default
    if (!next_token(":"))
        cerr << "缺少:" << endl;
    auto default_structure = make_default(statement());
    if (next_token(";")) {
        return default_structure;
    } else {
        cerr << "缺少;" << endl;
    }

    // 2023/2/23: todo
    return std::make_shared<StatementStructureBase>();
}

StatementStructure CaseStatement::operator()() {
    cout << "case" << endl;
    token = next_token(); // read default
    if (!next_token(":"))
        cerr << "缺少:" << endl;
    auto case_structure = make_case(0, 0, statement());
    if (next_token(";")) {
        return case_structure;
    } else {
        cerr << "缺少;" << endl;
    }

    // 2023/2/23: todo
    return std::make_shared<StatementStructureBase>();
}

StatementStructure ExpressionStatement::operator()() {
    cout << "expression statement" << endl;
    // 这里没法调用 以后再改把
    expression();
    // make_expression
    if (!next_token(";"))
        cerr << "缺少;" << endl;
    return nullptr;
}

StatementStructure ReturnStatement::operator()() {
    cout << "return" << endl;
    token = next_token(); // read return
    if (is_expression(token)) {
        expression();
        // make_expression
    }
    if (!next_token(";"))
        cerr << "缺少;" << endl;
    return make_return(0, nullptr);
}

// 这些跳转语句需要保存一个跳转的位置
// 但是怎么获得呢 ？？
// 不对 应该在调用调用这个函数的时候赋予自身的地址
// 比如我们在一个for语句内发现了一个break语句 那么
// 我们就需要把for语句指针传给break语句
StatementStructure BreakStatement::operator()() {
    cout << "break" << endl;
    token = next_token(); // read break
    if (!next_token(";"))
        cerr << "缺少;" << endl;
    return make_break(nullptr);
}

StatementStructure ContinueStatement::operator()() {
    cout << "continue" << endl;
    token = next_token(); // read continue
    if (!next_token(";"))
        cerr << "缺少;" << endl;
    // 这里同 break
    return make_continue(nullptr);
}

StatementStructure NullStatement::operator()() {
    cout << "null" << endl;
    if (!next_token(";"))
        cerr << "缺少;" << endl;
    return nullptr; // 没有必要为空语句单独开一个类型 浪费
}

// 真正复杂的句子
/*
        "if" '(' expression ')' statement
        "if" '(' expression ')' statement "else" statement
*/
StatementStructure IFStatement::operator()() {
    cout << "if statement" << endl;
    // 这里的else怎么处理的？？竟然是statement不处理else字句
    // 所以如果有else子句 一定存在于statement之后
    token = next_token(); // read if
    token = next_token(); // read  (

    // 这里的用法是不太对的 我们应该首先声明一个相应的句子类型
    // 然后在后序的分析中填充这个类型
    // 因为我们有可能在还没有完全填充句子的时候需要把句子的入口发送到其他位置
    // 所以我的实现基本上是错误的
    // 或者说我需要在加一个获取空的类的函数 然后把类成员作为public 这样比较方便
    // auto if_structure = std::make_shared<IfStatementStructure>();
    // 不对 我定义了构造函数 默认构造函数就没用了 这样是通不过编译的
    // 算了 就这么写把

    // 2023/2/23: todo
    std::shared_ptr<IfStatementStructure> if_structure = nullptr;

    if (is_expression(token)) {
        expression();
        // make_expression
        if_structure->condition = nullptr;
        if (next_token(")")) {
            if_structure->true_statement = statement();
            if (next_token("else")) {
                if_structure->false_statement = statement();
            }
        } else {
            cerr << "缺少)" << endl;
        }
    } else {
        cerr << "if语句需要一个expression" << endl;
    }
    return if_structure;
}

/*
        "while" '(' expression ')' statement
        "do" statement "while" '(' expression ')' ';'
        "for" '(' [ expression ] ';' [ expression ] ';' [ expression ] ')'
   statement

*/
StatementStructure WhileStatement::operator()() {
    cout << "while" << endl;
    token = next_token();
    // 2023/2/23
    // auto while_structure = std::make_shared<WhileStatementStructure>();
    std::shared_ptr<WhileStatementStructure> while_structure = nullptr;
    if (next_token("(")) {
        expression();
        while_structure->condition = nullptr;
        if (next_token(")")) {
            while_structure->iteration_statement = statement();
        } else {
            cerr << "缺少)" << endl;
        }
    } else {
        cerr << "缺少(" << endl;
    }
    return while_structure;
}

/*
"do" statement "while" '(' expression ')' ';'
*/
StatementStructure DoWhileStatement::operator()() {
    cout << "do while" << endl;
    token = next_token(); // read do
    // 2023/2/23: todo
    // auto do_while_structure = std::make_shared<DoWhileStatementStructure>();
    std::shared_ptr<DoWhileStatementStructure> do_while_structure = nullptr;
    do_while_structure->iteration_statement = statement();
    if (next_token("while")) {
        if (next_token("(")) {
            expression();
            do_while_structure->condition = nullptr;
            if (next_token(")")) {
                if (next_token(";")) {

                } else {
                    cerr << "缺少;" << endl;
                }
            } else {
                cerr << "缺少）" << endl;
            }
        } else {
            cerr << "缺少（" << endl;
        }
    } else {
        cerr << "缺少while" << endl;
    }
    return do_while_structure;
}

/*
"for" '(' [ expression ] ';' [ expression ] ';' [ expression ] ')' statement
*/
StatementStructure ForStatement::operator()() {
    cout << "for" << endl;
    token = next_token(); // 这里的逻辑不对 卧槽 我好像都写错了
    // 在执行完这一句之后 token = "("

    // 2023/2/23: todo
    // auto for_structure = std::make_shared<ForStatementStructure>();
    std::shared_ptr<ForStatementStructure> for_structure = nullptr;
    if (next_token("(")) // 而这里我判断的是下一个也就是 ) 后面的是不是）
                         // 显然不是 所以逻辑错误
    {
        expression(); // e1
        for_structure->initialization = nullptr;
        // 这里的嵌套if太多了 应该是告诉我要换一个思路来写这么复杂的嵌套if
        token = next_token(); // ;
        expression();         // e2
        for_structure->condition = nullptr;
        token = next_token(); // ;
        expression();         // e3
        for_structure->increment = nullptr;
        if (next_token(")")) {
            for_structure->iteration_statement = statement();
        } else {
            cerr << "缺少)" << endl;
        }
    } else {
        cerr << "缺少(" << endl;
    }
    return for_structure;
}

/*
"switch" '(' expression ')' statement
*/
StatementStructure SwitchStatement::operator()() {
    cout << "switch statement" << endl;
    token = next_token();
    // 2023/2/23: todo
    // auto switch_structure = std::make_shared<SwitchStatementStructure>();
    std::shared_ptr<SwitchStatementStructure> switch_structure = nullptr;
    if (next_token("(")) {
        expression();
        switch_structure->condition = nullptr;
        // 我应该直接在这里完成所有case和default的读取操作
        // 本来就不应该出现在别的地方
        // 而不是简单的读取一个statement就完事 不过也比较简单
        if (next_token(")")) {
            statement();
        } else {
            cerr << "缺少)" << endl;
        }
    } else {
        cerr << "缺少(" << endl;
    }
    return switch_structure;
}

_CML_END_
